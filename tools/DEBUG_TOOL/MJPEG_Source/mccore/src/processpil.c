/*******************************************************************
* Process Platform Independent Layer
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "processpil.h"

#define SHELL_BUFFER_SIZE 256

#ifdef WIN32
const int w32Priority[]={
	IDLE_PRIORITY_CLASS,
#if _WIN32_WINNT >= 0x0500
	BELOW_NORMAL_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS,
	ABOVE_NORMAL_PRIORITY_CLASS,
#else
	IDLE_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS,
#endif
	HIGH_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
};
#endif

int ShellRead(SHELL_PARAM* param, int timeout)
{
	int offset=0;
#ifdef WIN32
	DWORD dwRead;
#else
	int ret;
	fd_set fds;
	struct timeval tv;
#endif

	if (!param->buffer || param->iBufferSize <= 0) {
		if (param->flags & SHELL_ALLOC) {
			param->buffer=malloc(SHELL_BUFFER_SIZE);
			param->iBufferSize=SHELL_BUFFER_SIZE;
		} else {
			return -1;
		}
	}

#ifndef WIN32
	FD_ZERO(&fds);
	FD_SET(param->fdRead,&fds);
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout - tv.tv_sec * 1000) * 1000;
#endif

	if (param->iDelimiter) {
		for(;;) {
#ifdef WIN32
			if (!PeekNamedPipe(param->fdRead,0,0,0,0,0) ||
					!ReadFile(param->fdRead, param->buffer+offset, 1, &dwRead, NULL))
				return -1;
#else
			ret=select(param->fdRead+1,&fds,NULL,NULL,&tv);
			if (ret<1) break;
			ret=read(param->fdRead,param->buffer+offset,1);
			if (ret<1) break;;
#endif
			if ((int)param->buffer[offset++]==param->iDelimiter) break;
			if (offset==param->iBufferSize-1) {
				if  (!(param->flags & SHELL_ALLOC)) break;
				param->iBufferSize+=SHELL_BUFFER_SIZE;
				param->buffer=realloc(param->buffer,param->iBufferSize);
			}
		}
		param->buffer[offset]=0;
#ifdef WIN32
		return 0;
#else
		return (ret>0)?offset:-1;
#endif
	} else if (!(param->flags & SHELL_READ_STDOUT_ALL)) {
#ifdef WIN32
		if (!PeekNamedPipe(param->fdRead,0,0,0,0,0) ||
				!ReadFile(param->fdRead, param->buffer, param->iBufferSize - 1, &dwRead, NULL))
			return -1;
		param->buffer[dwRead]=0;
		return dwRead;
#else
		if (select(param->fdRead+1,&fds,NULL,NULL,&tv) < 1) return -1;
		ret=read(param->fdRead,param->buffer, param->iBufferSize - 1);
		if (ret<1) return -1;
		param->buffer[ret] = 0;
		return ret;
#endif
	} else {
		int fSuccess;
		for(;;) {
			int i;
			if (offset >= param->iBufferSize - 1) {
				if (param->flags & SHELL_ALLOC) {
					param->iBufferSize <<= 1;
					param->buffer = realloc(param->buffer, param->iBufferSize);
				} else {
					offset = 0;
				}
			}
			fSuccess = (PeekNamedPipe(param->fdRead,0,0,0,0,0) &&
				ReadFile(param->fdRead, param->buffer + offset, param->iBufferSize - 1 - offset, &dwRead, NULL));
			if (!fSuccess) break;
			// convert line-feed
			if (param->flags & SHELL_CONVERT_LF) {
				for (i = 0; i < (int)dwRead; i++) {
					if (*(param->buffer + offset + i) == '\n' && *(param->buffer + offset + i - 1) != '\r') {
						if (offset + (int)dwRead >= param->iBufferSize - 2) {
							if (param->flags & SHELL_ALLOC) {
								param->iBufferSize <<= 1;
								param->buffer = realloc(param->buffer, param->iBufferSize);
							} else {
								break;
							}
						}
						memmove(param->buffer + offset + i + 1, param->buffer + offset + i, dwRead - i);
						param->buffer[offset + i] = '\r';
						i++;
						dwRead++;
					}
				}
			}
			offset += dwRead;
		}
		param->buffer[offset]=0;
		return offset;
	}
}

int ShellTerminate(SHELL_PARAM* param)
{
	int ret;
#ifdef WIN32
	if (!param->piProcInfo.hProcess) return -1;
	ret=TerminateProcess(param->piProcInfo.hProcess,0)?0:-1;
#else
	if (!param->pid) return 0;
	ret=kill(param->pid,SIGSTOP);
	param->pid=0;
#endif
	return ret;
}

void ShellClean(SHELL_PARAM* param)
{
#ifdef WIN32
	//CloseHandle((HANDLE)param->fdRead);
	//CloseHandle((HANDLE)param->fdWrite);
	CloseHandle(param->piProcInfo.hProcess);
	CloseHandle(param->piProcInfo.hThread);
	param->piProcInfo.hProcess=NULL;
#else
	close(param->fdRead);
	close(param->fdWrite);
	param->pid=0;
#endif
	param->fdRead=0;
	param->fdWrite=0;
	if (param->flags & SHELL_ALLOC) {
		free(param->buffer);
		param->buffer=NULL;
		param->iBufferSize=0;
	}
}

int ShellWait(SHELL_PARAM* param, int iTimeout)
{
#ifdef WIN32
	switch (WaitForSingleObject(param->piProcInfo.hProcess,(iTimeout==-1)?INFINITE:iTimeout)) {
	case WAIT_OBJECT_0:
		return 1;
	case WAIT_TIMEOUT:
		return 0;
	default:
		return -1;
	}
#else
	int ret=-1;
	waitpid(param->pid,&ret,0);
	return ret;
#endif
}

int ShellSetPriority(SHELL_PARAM* param, int iPriority)
{
#ifdef WIN32
	if (iPriority<6 && param->piProcInfo.hProcess)
		return SetPriorityClass(param->piProcInfo.hProcess,w32Priority[iPriority])?0:-1;
	else
		return -1;
#else
	return -1;
#endif
}

int ShellExec(SHELL_PARAM* param)
{
#ifdef WIN32
	SECURITY_ATTRIBUTES saAttr={sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	STARTUPINFO siStartInfo;
	BOOL fSuccess;
	TCHAR newPath[512],prevPath[512];
	HANDLE hChildStdinRd, hChildStdinWr, hChildStdoutRd, hChildStdoutWr;
#else
	int fdin[2], fdout[2],pid,i;
	int fdStdinChild;
	int fdStdoutChild;
	TCHAR **args=NULL,*argString=NULL, *p;
	TCHAR *filePath;
 	TCHAR *newPath,*prevPath,*env[2];
#endif

#ifdef WIN32
	if (param->piProcInfo.hProcess || !param->pchCommandLine) return -1;

	_setmode( _fileno( stdin ), _O_BINARY );
	_setmode( _fileno( stdout ), _O_BINARY );

	// modify path variable
	if (param->pchPath) {
		GetEnvironmentVariable("PATH",prevPath,sizeof(prevPath));
		sprintf(newPath,"%s;s",param->pchPath,prevPath);
		SetEnvironmentVariable("PATH",newPath);
	}
	
	memset( &param->piProcInfo, 0, sizeof(PROCESS_INFORMATION) );
	memset( &siStartInfo, 0, sizeof(STARTUPINFO) );

///////////////////////////////////////////////////////////////////////
//	create redirecting pipes
///////////////////////////////////////////////////////////////////////

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	if (param->flags & SHELL_REDIRECT_STDOUT) {
		// Create a pipe for the child process's STDOUT.
		if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
			return -1;
		}
		// Create noninheritable read handle and close the inheritable read
		// handle.
		fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
			GetCurrentProcess(), (LPHANDLE)&param->fdRead, 0,
				FALSE,
				DUPLICATE_SAME_ACCESS);
		if( !fSuccess ) return 0;
		CloseHandle(hChildStdoutRd);
		siStartInfo.hStdError = hChildStdoutWr;
		siStartInfo.hStdOutput = hChildStdoutWr;
	}
	if (param->flags & SHELL_REDIRECT_STDERR) {
		siStartInfo.hStdError = siStartInfo.hStdOutput;
	}
	siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	if (param->flags & SHELL_REDIRECT_STDIN) {
		// Create a pipe for the child process's STDIN.
		if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) return 0;

		// Duplicate the write handle to the pipe so it is not inherited.
		fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr,
			GetCurrentProcess(), (LPHANDLE)&param->fdWrite, 0,
				FALSE,                  // not inherited
				DUPLICATE_SAME_ACCESS);
		if (! fSuccess) return -1;
		CloseHandle(hChildStdinWr);
		siStartInfo.hStdInput = hChildStdinRd;
	}

	// Set up members of the STARTUPINFO structure.
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.wShowWindow=(param->flags & SHELL_SHOW_WINDOW)?SW_SHOW:SW_HIDE;
	if (siStartInfo.hStdInput || siStartInfo.hStdOutput)
		siStartInfo.dwFlags = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	else
		siStartInfo.dwFlags = STARTF_USESHOWWINDOW;

///////////////////////////////////////////////////////////////////////
//	create child process
///////////////////////////////////////////////////////////////////////
// Create the child process.

	{
		TCHAR* appname = NULL;
		TCHAR* p = strrchr(param->pchCommandLine, '.');
		if (p && !stricmp(p + 1, "bat")) {
			appname = "cmd.exe";
		}
#if 0
		TCHAR* p;	
		TCHAR* s = param->pchCommandLine;
		int len = 0;

		if (s[0] == '\"') {
			p = strchr(s + 1, '\"');
			if (p) len = p - s - 1;
			s++;
		} else {
			p = strchr(s, ' ');
			if (p) {
				len = p - s;
			} else {
				len = strlen(s);
			}
		}
		appname = malloc(len > 7 ? len + 1 : 8);
		memcpy(appname, s, len);
		appname[len] = 0;
		if (!stricmp(appname + len - 4, ".bat")) {
			strcpy(appname, "cmd.exe");
		}
#endif

		fSuccess = CreateProcess(appname,
			param->pchCommandLine, // command line
			NULL,          // process security attributes
			NULL,          // primary thread security attributes
			TRUE,          // handles are inherited
			0,      // creation flags
			NULL,			 // process' environment
			param->pchCurDir,      // current directory
			&siStartInfo,  // STARTUPINFO pointer
			&param->piProcInfo);  // receives PROCESS_INFORMATION

		free(appname);
	}

	if (param->pchPath) SetEnvironmentVariable("PATH",prevPath);
	if (!fSuccess) return -1;
	WaitForInputIdle(param->piProcInfo.hProcess,INFINITE);

	if (param->flags & SHELL_REDIRECT_STDIN)
		CloseHandle(hChildStdinRd);
	if (param->flags & SHELL_REDIRECT_STDOUT)
		CloseHandle(hChildStdoutWr);
#else

	if (param->pid) return -1;

	if (param->flags & SHELL_REDIRECT_STDIN) {
		pipe(fdin);
		param->fdWrite = fdin[1];
		fdStdinChild=fdin[0];
	}
	if (param->flags & SHELL_REDIRECT_STDOUT) {
		pipe(fdout);
		param->fdRead = fdout[0];
		fdStdoutChild=fdout[1];
	}
	
	pid = fork();
	if (pid == -1) return -1;
	if (pid == 0) { /* chid process */
		//generate argument list
		for (p=param->pchCommandLine,i=2;*p;p++) {
			if (*p==' ') i++;
		}
		args=malloc(i*sizeof(TCHAR*));
		argString=strdup(param->pchCommandLine);
		i=0;
		if (argString) {
			p=strtok(argString," ");
			while (p) {
				args[i++]=p;
				p=strtok(NULL," ");
			}
		}
		args[i]=NULL;
		p=strrchr(args[0],'/');
		if (p) {
			filePath=args[0];
			args[0]=p+1;
		} else {
			filePath=NULL;
		}

		//set PATH
		env[0]=NULL;
		if (param->pchPath) {
			prevPath=getenv("PATH");
			env[0]=malloc(strlen(prevPath)+strlen(param->pchPath)+2+5);
			sprintf(env[0],"PATH=%s:%s",prevPath,param->pchPath);
			env[1]=NULL;
		}
		if (param->flags & SHELL_REDIRECT_STDIN) {
			close(fdin[1]);
			dup2(fdStdinChild, 0);
		}
		if (param->flags & SHELL_REDIRECT_STDOUT) {
			close(fdout[0]);
			dup2(fdStdoutChild, 1);
		}
		if (execve(filePath, args, env)<0) {
			printf("Error starting specified program\n");
		}
		return 0;
	}
	param->pid=pid;
#endif
	return 0;
}
