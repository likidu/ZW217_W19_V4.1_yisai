

#include <stdio.h>
#include <string.h>
//#include <memory.h>
#include "helix_types.h"
#include "helix_result.h"
#include "rm_parser_internal.h"
//#include "rm_memory_default.h"
#include "memory_utils.h"
#include "pack_utils.h"
#include "string_utils.h"
#include "rm_stream.h"
#include "packet_defines.h"

#define RM_PARSER_LOGICAL_PREFIX    "logical-"
#define RM_PARSER_LOGICAL_FILEINFO  "logical-fileinfo"
#define RM_PARSER_NUM_RULE_BOOKS          9
#define RM_PARSER_RULE_BOOK_BUFFER_SIZE 320 /* Big enough for format string and args */

static const char* g_pszRuleBook[] =
{
    /* VBR Real streams when max_bit_rate != 0 */
    "priority=%lu,AverageBandwidth=%lu,MaximumBandwidth=%lu,"
    "TimeStampDelivery=TRUE,PNMKeyFrameRule=T;"        /*lint !e786*/
    "OnDepend=\"0\",priority=%lu,AverageBandwidth=0,MaximumBandwidth=0," /*lint !e786*/
    "TimeStampDelivery=TRUE,PNMNonKeyFrameRule=T;", /*lint !e786*/
    /* VBR Real streams when max_bit_rate == 0 */
    "priority=%lu,AverageBandwidth=%lu,TimeStampDelivery=TRUE,PNMKeyFrameRule=T;"
    "OnDepend=\"0\",priority=%lu,AverageBandwidth=0,TimeStampDelivery=TRUE,PNMNonKeyFrameRule=T;", /*lint !e786*/
    /* Non-real-datatype VBR streams when max_bit_rate != 0 */
    "Marker=0,AverageBandwidth=%lu,MaximumBandwidth=%lu,TimeStampDelivery=TRUE;"
    "Marker=1,AverageBandwidth=0,MaximumBandwidth=0,TimeStampDelivery=TRUE;", /*lint !e786*/
    /* Non-real-datatype VBR streams when max_bit_rate == 0 && avg_bit_rate != 0 */
    "Marker=0,AverageBandwidth=%lu,TimeStampDelivery=TRUE;"
    "Marker=1,AverageBandwidth=0,TimeStampDelivery=TRUE;", /*lint !e786*/
    /* Non-real-datatype VBR streams when max_bit_rate == 0 && avg_bit_rate == 0 */
    "Marker=0,TimeStampDelivery=TRUE;"
    "Marker=1,TimeStampDelivery=TRUE;", /*lint !e786*/
    /* CBR RealVideo */
    "#($Bandwidth >= %lu),priority=9,averagebandwidth=%lu,PNMKeyFrameRule=T;"
    "#($Bandwidth >= %lu),OnDepend=\"0\",priority=5,averagebandwidth=0,PNMNonKeyFrameRule=T;" /*lint !e786*/
    "#($Bandwidth < %lu),priority=9,timestampdelivery=T,DropByN=T,PNMThinningRule=T;", /*lint !e786*/
    /* RealEvent streams */
    "TimeStampDelivery=T,priority=10,PNMKeyFrameRule=T;"
    "TimeStampDelivery=T,priority=10,PNMNonKeyFrameRule=T;", /*lint !e786*/
    /* CBR RealAudio streams */
    "priority=5,averagebandwidth=%lu,PNMKeyFrameRule=T;"
    "priority=5,averagebandwidth=0,PNMNonKeyFrameRule=T,OnDepend=\"0\",OffDepend=\"0\";", /*lint !e786*/
    /* CBR non-real-datatype streams */
    "Marker=0,priority=5,averagebandwidth=%lu;"
    "Marker=1,priority=5,averagebandwidth=0;" /*lint !e786*/
};

HX_RESULT rm_parseri_unpack_generic_hdr(rm_parser_internal* pInt, struct rm_generic_hdr* h)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && h)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= 10)
        {
            /* Unpack the rm_generic_hdr */
            h->id      = rm_unpack32(&pBuf, &ulLen);
            h->size    = rm_unpack32(&pBuf, &ulLen);
            h->version = rm_unpack16(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}


HX_RESULT rm_parseri_unpack_file_hdr(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= 18)
        {
            /* Unpack the rm_file_hdr */
            pInt->fileHdr.id           = rm_unpack32(&pBuf, &ulLen);
            pInt->fileHdr.size         = rm_unpack32(&pBuf, &ulLen);
            pInt->fileHdr.version      = rm_unpack16(&pBuf, &ulLen);
            pInt->fileHdr.file_version = rm_unpack32(&pBuf, &ulLen);
            pInt->fileHdr.num_headers  = rm_unpack32(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_unpack_properties_hdr(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= 50)
        {
            /* Unpack the rm_properties_hdr */
            pInt->propHdr.id           = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.size         = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.version      = rm_unpack16(&pBuf, &ulLen);
            pInt->propHdr.max_bit_rate = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.avg_bit_rate = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.max_pkt_size = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.avg_pkt_size = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.num_pkts     = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.duration     = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.preroll      = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.index_offset = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.data_offset  = rm_unpack32(&pBuf, &ulLen);
            pInt->propHdr.num_streams  = rm_unpack16(&pBuf, &ulLen);
            pInt->propHdr.flags        = rm_unpack16(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_unpack_content_hdr(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= 12)
        {
            /* Unpack the rm_content_hdr */
            pInt->contHdr.id       = rm_unpack32(&pBuf, &ulLen);
            pInt->contHdr.size     = rm_unpack32(&pBuf, &ulLen);
            pInt->contHdr.version  = rm_unpack16(&pBuf, &ulLen);
            pInt->contHdr.title_sz = rm_unpack16(&pBuf, &ulLen);
            if (ulLen >= (UINT32) pInt->contHdr.title_sz + 2)
            {
                rm_unpack_string(&pBuf, &ulLen, pInt->contHdr.title_sz,
                                &pInt->contHdr.title, pInt->pUserMem,
                                pInt->fpMalloc, pInt->fpFree);
                pInt->contHdr.author_sz = rm_unpack16(&pBuf, &ulLen);
                if (ulLen >= (UINT32) pInt->contHdr.author_sz + 2)
                {
                    rm_unpack_string(&pBuf, &ulLen, pInt->contHdr.author_sz,
                                    &pInt->contHdr.author, pInt->pUserMem,
                                    pInt->fpMalloc, pInt->fpFree);
                    pInt->contHdr.copyright_sz = rm_unpack16(&pBuf, &ulLen);
                    if (ulLen >= (UINT32) pInt->contHdr.copyright_sz + 2)
                    {
                        rm_unpack_string(&pBuf, &ulLen, pInt->contHdr.copyright_sz,
                                        &pInt->contHdr.copyright, pInt->pUserMem,
                                        pInt->fpMalloc, pInt->fpFree);
                        pInt->contHdr.comment_sz = rm_unpack16(&pBuf, &ulLen);
                        if (ulLen >= pInt->contHdr.comment_sz)
                        {
                            rm_unpack_string(&pBuf, &ulLen, pInt->contHdr.comment_sz,
                                            &pInt->contHdr.comment, pInt->pUserMem,
                                            pInt->fpMalloc, pInt->fpFree);
                            /* Clear the return value */
                            retVal = HXR_OK;
                        }
                    }
                }
            }
        }
    }

    return retVal;
}

void rm_parseri_cleanup_content_hdr(rm_parser_internal* pInt)
{
    if (pInt)
    {
        /* Delete the title string */
        if (pInt->contHdr.title)
        {
            rm_parseri_free(pInt, pInt->contHdr.title);
            pInt->contHdr.title = HXNULL;
        }
        /* Delete the author string */
        if (pInt->contHdr.author)
        {
            rm_parseri_free(pInt, pInt->contHdr.author);
            pInt->contHdr.author = HXNULL;
        }
        /* Delete the copyright string */
        if (pInt->contHdr.copyright)
        {
            rm_parseri_free(pInt, pInt->contHdr.copyright);
            pInt->contHdr.copyright = HXNULL;
        }
        /* Delete the comment string */
        if (pInt->contHdr.comment)
        {
            rm_parseri_free(pInt, pInt->contHdr.comment);
            pInt->contHdr.comment = HXNULL;
        }
    }
}

HX_RESULT rm_parseri_unpack_media_props_hdr(rm_parser_internal* pInt,
                                            struct rm_media_props_hdr* h)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && h)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        /* Unpack the rm_media_props_hdr */
        if (ulLen >= 41)
        {
            h->id             = rm_unpack32(&pBuf, &ulLen);
            h->size           = rm_unpack32(&pBuf, &ulLen);
            h->version        = rm_unpack16(&pBuf, &ulLen);
            h->stream_num     = rm_unpack16(&pBuf, &ulLen);
            h->max_bit_rate   = rm_unpack32(&pBuf, &ulLen);
            h->avg_bit_rate   = rm_unpack32(&pBuf, &ulLen);
            h->max_pkt_size   = rm_unpack32(&pBuf, &ulLen);
            h->avg_pkt_size   = rm_unpack32(&pBuf, &ulLen);
            h->start_time     = rm_unpack32(&pBuf, &ulLen);
            h->preroll        = rm_unpack32(&pBuf, &ulLen);
            h->duration       = rm_unpack32(&pBuf, &ulLen);
            h->stream_name_sz = rm_unpack8(&pBuf,  &ulLen);
            if (ulLen >= (UINT32) h->stream_name_sz + 1)
            {
                rm_unpack_string(&pBuf, &ulLen, h->stream_name_sz, &h->stream_name,
                                pInt->pUserMem, pInt->fpMalloc, pInt->fpFree);
                h->mime_type_sz = rm_unpack8(&pBuf, &ulLen);
                if (ulLen >= (UINT32) h->mime_type_sz + 4)
                {
                    rm_unpack_string(&pBuf, &ulLen, h->mime_type_sz, &h->mime_type,
                                    pInt->pUserMem, pInt->fpMalloc, pInt->fpFree);
                    h->type_spec_sz = rm_unpack32(&pBuf, &ulLen);
                    if (ulLen >= (UINT32) h->type_spec_sz)
                    {
                        rm_unpack_buffer(&pBuf, &ulLen, h->type_spec_sz, &h->type_spec,
                                        pInt->pUserMem, pInt->fpMalloc, pInt->fpFree);
                        /* Clear the return value */
                        retVal = HXR_OK;
                    }
                }
            }
        }
    }

    return retVal;
}

void rm_parseri_cleanup_media_props_hdr(rm_parser_internal* pInt,
                                         struct rm_media_props_hdr* h)
{
    if (pInt && h)
    {
        /* Delete the stream name string */
        if (h->stream_name)
        {
            rm_parseri_free(pInt, h->stream_name);
            h->stream_name = HXNULL;
        }
        /* Delete the mime type string */
        if (h->mime_type)
        {
            rm_parseri_free(pInt, h->mime_type);
            h->mime_type = HXNULL;
        }
        /* Delete the type specific data buffer */
        if (h->type_spec)
        {
            rm_parseri_free(pInt, h->type_spec);
            h->type_spec = HXNULL;
        }
    }
}

void rm_parseri_cleanup_all_media_props_hdrs(rm_parser_internal* pInt)
{
    if (pInt && pInt->pMediaPropsHdr)
    {
        /* Clean up each individual media properties header */
        UINT32 i = 0;
        for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
        {
            rm_parseri_cleanup_media_props_hdr(pInt, &pInt->pMediaPropsHdr[i]);
        }
        /* Free the memory for the array */
        rm_parseri_free(pInt, pInt->pMediaPropsHdr);
        /* Zero out the number of media props headers */
        pInt->ulNumMediaPropsHdrs      = 0;
        pInt->ulNumMediaPropsHdrsAlloc = 0;
        /* Null out the pointer */
        pInt->pMediaPropsHdr = HXNULL;
    }
}

HX_RESULT rm_parseri_unpack_name_value_map(rm_parser_internal* pInt,
                                           struct rm_name_value_map* pMap,
                                           BYTE** ppBuf, UINT32* pulLen)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pMap && ppBuf && pulLen && *pulLen >= 7)
    {
        /* Unpack the name-value map struct */
        pMap->size    = rm_unpack32(ppBuf, pulLen);
        pMap->version = rm_unpack16(ppBuf, pulLen);
        pMap->name_sz = rm_unpack8(ppBuf, pulLen);
        if (*pulLen >= (UINT32) pMap->name_sz + 6)
        {
            rm_unpack_string(ppBuf, pulLen, pMap->name_sz, &pMap->name,
                            pInt->pUserMem, pInt->fpMalloc, pInt->fpFree);
            pMap->type     = rm_unpack32(ppBuf, pulLen);
            pMap->value_sz = rm_unpack16(ppBuf, pulLen);
            if (*pulLen >= pMap->value_sz)
            {
                rm_unpack_buffer(ppBuf, pulLen, pMap->value_sz, &pMap->value,
                                pInt->pUserMem, pInt->fpMalloc, pInt->fpFree);
                /* Clear the return value */
                retVal = HXR_OK;
            }
        }
    }

    return retVal;
}

void rm_parseri_cleanup_name_value_map(rm_parser_internal* pInt,
                                       struct rm_name_value_map* h)
{
    if (pInt && h)
    {
        /* Delete the name string */
        if (h->name)
        {
            rm_parseri_free(pInt, h->name);
            h->name = HXNULL;
        }
        /* Delete the value buffer */
        if (h->value)
        {
            rm_parseri_free(pInt, h->value);
            h->value = HXNULL;
        }
    }
}

void rm_parseri_cleanup_all_name_value_maps(rm_parser_internal* pInt,
                                            struct rm_logical_stream_hdr* h)
{
    if (pInt && h && h->props)
    {
        /* Clean up each name-value pair */
        UINT16 i = 0;
        for (i = 0; i < h->num_props; i++)
        {
            rm_parseri_cleanup_name_value_map(pInt, &h->props[i]);
        }
        /* Free the array of props */
        rm_parseri_free(pInt, h->props);
        /* Zero out the number of props */
        h->num_props = 0;
        /* NULL out the pointer */
        h->props = HXNULL;
    }
}

HX_RESULT rm_parseri_unpack_logical_stream_hdr(rm_parser_internal* pInt,
                                               struct rm_media_props_hdr* mh,
                                               struct rm_logical_stream_hdr* h)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && mh && mh->type_spec && mh->type_spec_sz > 0 && h)
    {
        /* Assign temporary variables */
        BYTE*  pBuf   = mh->type_spec;
        UINT32 ulLen  = mh->type_spec_sz;
        UINT32 ulSize = 0;
        UINT16 i      = 0;
        /* Parse the logical stream header */
        if (ulLen >= 8)
        {
            h->size                 = rm_unpack32(&pBuf, &ulLen);
            h->version              = rm_unpack16(&pBuf, &ulLen);
            h->num_physical_streams = rm_unpack16(&pBuf, &ulLen);
            if (ulLen >= (UINT32) 6 * h->num_physical_streams + 2)
            {
                rm_unpack_array(&pBuf, &ulLen, h->num_physical_streams, sizeof(UINT16),
                                (void**) &h->physical_stream_num, pInt->pUserMem,
                                pInt->fpMalloc, pInt->fpFree);
                rm_unpack_array(&pBuf, &ulLen, h->num_physical_streams, sizeof(UINT32),
                                (void**) &h->data_offsets, pInt->pUserMem,
                                pInt->fpMalloc, pInt->fpFree);
                h->num_rules = rm_unpack16(&pBuf, &ulLen);
                if (ulLen >= (UINT32) h->num_rules * 2 + 2)
                {
                    rm_unpack_array(&pBuf, &ulLen, h->num_rules, sizeof(UINT16),
                                    (void**) &h->rule_stream_map, pInt->pUserMem,
                                    pInt->fpMalloc, pInt->fpFree);
                    h->num_props = rm_unpack16(&pBuf, &ulLen);
                    /* Clean up any existing name-value pairs */
                    rm_parseri_cleanup_all_name_value_maps(pInt, h);
                    /* Allocate an array of name value pairs */
                    ulSize = h->num_props * sizeof(struct rm_name_value_map);
                    if (ulSize)
                    {
                        h->props = (struct rm_name_value_map*) rm_parseri_malloc(pInt, ulSize);
                        if (h->props)
                        {
                            /* NULL out the memory */
                            memset(h->props, 0, ulSize);
                            /* Clear the return value */
                            retVal = HXR_OK;
                            /* Unpack each of the name-value pairs */
                            for (i = 0; i < h->num_props && retVal == HXR_OK; i++)
                            {
                                retVal = rm_parseri_unpack_name_value_map(pInt, &h->props[i],
                                                                          &pBuf, &ulLen);
                            }
                        }
                    }
                    else
                    {
                        /* No properties - not an error */
                        retVal = HXR_OK;
                    }
                }
            }
        }
    }

    return retVal;
}

UINT32 rm_parseri_get_num_logical_streams(rm_parser_internal* pInt)
{
    UINT32 ulRet = 0;

    if (pInt && pInt->pMediaPropsHdr)
    {
        UINT32 i = 0;
        for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
        {
            const char* pszMimeType = (const char*) pInt->pMediaPropsHdr[i].mime_type;
            if (rm_parseri_is_logical_stream_mime_type(pszMimeType))
            {
                ulRet++;
            }
        }
    }

    return ulRet;
}

HXBOOL rm_parseri_is_logical_fileinfo_present(rm_parser_internal* pInt, UINT32* pulIndx)
{
    HXBOOL bRet = FALSE;

    if (pInt && pInt->pMediaPropsHdr && pulIndx)
    {
        UINT32 i = 0;
        for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
        {
            const char* pszMimeType = (const char*) pInt->pMediaPropsHdr[i].mime_type;
            if (rm_parseri_is_logical_fileinfo_mime_type(pszMimeType))
            {
                bRet     = TRUE;
                *pulIndx = i;
                break;
            }
        }
    }

    return bRet;
}

void rm_parseri_cleanup_logical_fileinfo_hdr(rm_parser_internal* pInt)
{
    if (pInt && pInt->pLogicalFileInfo)
    {
        /* Clean up the struct */
        rm_parseri_cleanup_logical_stream_hdr(pInt, pInt->pLogicalFileInfo);
        /* Free the struct memory */
        rm_parseri_free(pInt, pInt->pLogicalFileInfo);
        /* NULL out the pointer */
        pInt->pLogicalFileInfo = HXNULL;
    }
}

HX_RESULT rm_parseri_unpack_all_logical_stream_hdrs(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Declare some local variables we need later */
        UINT32 ulSize = 0;
        UINT32 ulIndx = 0;
        UINT32 i      = 0;
        /* Get the number of media props headers that are logical streams */
        UINT32 ulNumLogical = rm_parseri_get_num_logical_streams(pInt);
        if (ulNumLogical)
        {
            /* Free any existing logical stream array */
            rm_parseri_cleanup_all_logical_stream_hdrs(pInt);
            /* Compute the size of the logical stream header array */
            ulSize = ulNumLogical * sizeof(struct rm_logical_stream_hdr);
            /* Allocate the array of logical stream headers */
            pInt->pLogicalStreamHdr =
                (struct rm_logical_stream_hdr*) rm_parseri_malloc(pInt, ulSize);
            if (pInt->pLogicalStreamHdr)
            {
                /* Zero out the memory */
                memset(pInt->pLogicalStreamHdr, 0, ulSize);
                /* Assign the number of logical stream headers */
                pInt->ulNumLogicalStreamHdrs = ulNumLogical;
                /* Clear the return value */
                retVal = HXR_OK;
                /*
                 * Loop through all the media properties headers, and
                 * if a header is a logical stream, then unpack it
                 */
                for (i = 0; i < pInt->ulNumMediaPropsHdrs && retVal == HXR_OK; i++)
                {
                    const char* pszMimeType = (const char*) pInt->pMediaPropsHdr[i].mime_type;
                    if (rm_parseri_is_logical_stream_mime_type(pszMimeType))
                    {
                        /*
                         * Unpack the logical stream. We don't have
                         * to worry about ulIndx being less than
                         * ulNumLogicalStreamHdrs since we already 
                         * guaranteed above that ulNumLogicalStreamHdrs
                         * was correct.
                         */
                        retVal = rm_parseri_unpack_logical_stream_hdr(pInt,
                                                                      &pInt->pMediaPropsHdr[i],
                                                                      &pInt->pLogicalStreamHdr[ulIndx]);
                        if (retVal == HXR_OK)
                        {
                            ulIndx++;
                        }
                        else
                        {
                            /* Call back to the error interface */
                            rm_parseri_error(pInt, retVal, "Failed to parse logical stream header.");
                        }
                    }
                }
            }
        }
        else
        {
            /* No logical streams - single-rate file */
            retVal = HXR_OK;
        }
        if (retVal == HXR_OK)
        {
            /* Clean up any existing logical-fileinfo header */
            rm_parseri_cleanup_logical_fileinfo_hdr(pInt);
            /* Do we have a logical-fileinfo header? */
            if (rm_parseri_is_logical_fileinfo_present(pInt, &ulIndx))
            {
                /* Compute the size of the logical fileinfo */
                ulSize = sizeof(struct rm_logical_stream_hdr);
                /* Allocate space for the struct */
                pInt->pLogicalFileInfo =
                    (struct rm_logical_stream_hdr*) rm_parseri_malloc(pInt, ulSize);
                if (pInt->pLogicalFileInfo)
                {
                    /* Zero out the memory */
                    memset(pInt->pLogicalFileInfo, 0, ulSize);
                    /* Unpack the logical-fileinfo struct */
                    retVal = rm_parseri_unpack_logical_stream_hdr(pInt,
                                                                  &pInt->pMediaPropsHdr[ulIndx],
                                                                  pInt->pLogicalFileInfo);
                    if (retVal != HXR_OK)
                    {
                        /* Call back to the error interface */
                        rm_parseri_error(pInt, retVal, "Failed to parse logical-fileinfo header.");
                    }
                }
                else
                {
                    retVal = HXR_OUTOFMEMORY;
                }
            }
        }
    }

    return retVal;
}

void rm_parseri_cleanup_logical_stream_hdr(rm_parser_internal* pInt,
                                           struct rm_logical_stream_hdr* h)
{
    if (pInt && h)
    {
        /* Delete the stream number array */
        if (h->physical_stream_num)
        {
            rm_parseri_free(pInt, h->physical_stream_num);
            h->physical_stream_num = HXNULL;
        }
        /* Delete the data offsets array */
        if (h->data_offsets)
        {
            rm_parseri_free(pInt, h->data_offsets);
            h->data_offsets = HXNULL;
        }
        /* Delete the rule-to-stream map */
        if (h->rule_stream_map)
        {
            rm_parseri_free(pInt, h->rule_stream_map);
            h->rule_stream_map = HXNULL;
        }
        /* Clean up the logical stream props */
        rm_parseri_cleanup_all_name_value_maps(pInt, h);
        /* Free the memory of thepLogicalFileInfo */
        rm_parseri_free(pInt, pInt->pLogicalFileInfo);
        /* NULL out the pointer */
        pInt->pLogicalFileInfo = HXNULL;
    }
}

void rm_parseri_cleanup_all_logical_stream_hdrs(rm_parser_internal* pInt)
{
    if (pInt && pInt->pLogicalStreamHdr)
    {
        /* Clean up the individual logical stream headers */
        UINT32 i = 0;
        for (i = 0; i < pInt->ulNumLogicalStreamHdrs; i++)
        {
            rm_parseri_cleanup_logical_stream_hdr(pInt, &pInt->pLogicalStreamHdr[i]);
        }
        /* Free the memory of the array */
        rm_parseri_free(pInt, pInt->pLogicalStreamHdr);
        /* NULL out the array pointer */
        pInt->pLogicalStreamHdr = HXNULL;
        /* Zero out the array size and number of headers */
        pInt->ulNumLogicalStreamHdrs = 0;
    }
}

HX_RESULT rm_parseri_allocate_media_props_hdrs(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_OUTOFMEMORY;

    if (pInt)
    {
        UINT32 ulSize = 0;
        /* Clean up any existing media properties header array */
        rm_parseri_cleanup_all_media_props_hdrs(pInt);
        /*
         * Compute the number of stream properties headers
         * we need to allocate. Normally this is propHdr.num_streams,
         * but for file version > 0, it is possible to have file-info
         * header for each stream but not all of them may be present.
         */
        pInt->ulNumMediaPropsHdrsAlloc = pInt->propHdr.num_streams;
        if (pInt->fileHdr.file_version != 0)
        {
            if (pInt->ulNumMediaPropsHdrsAlloc > 2)
            {
                pInt->ulNumMediaPropsHdrsAlloc = (pInt->ulNumMediaPropsHdrsAlloc - 1) * 2;
            }
        }
        /* Calculate the size of the stream property header array */
        ulSize = pInt->ulNumMediaPropsHdrsAlloc * sizeof(struct rm_media_props_hdr);
        /* Allocate the number of media properties headers */
        pInt->pMediaPropsHdr =
            (struct rm_media_props_hdr*) rm_parseri_malloc(pInt, ulSize);
        if (pInt->pMediaPropsHdr)
        {
            /* NULL out the memory */
            memset((void*) pInt->pMediaPropsHdr, 0, ulSize);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

void rm_parseri_cleanup_read_buffer(rm_parser_internal* pInt)
{
    if (pInt && pInt->pReadBuffer)
    {
        rm_parseri_free(pInt, pInt->pReadBuffer);
        pInt->pReadBuffer      = HXNULL;
        pInt->ulReadBufferSize = 0;
        pInt->ulNumBytesRead   = 0;
    }
}

UINT32 rm_parseri_get_media_props_hdr_stream_num(rm_parser_internal* pInt)
{
    UINT32 ulRet = 0;

    if (pInt && pInt->pReadBuffer && pInt->ulNumBytesRead >= 12)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        /* Skip directly to the stream_num */
        pBuf  += 10;
        ulLen -= 10;
        /* Unpack the stream_num. */
        ulRet = (UINT32) rm_unpack16(&pBuf, &ulLen);
    }

    return ulRet;
}

HXBOOL rm_parseri_is_logical_stream_mime_type(const char* pszMimeType)
{
    HXBOOL bRet = FALSE;

    if (pszMimeType)
    {
        if (!rm_parseri_is_logical_fileinfo_mime_type(pszMimeType))
        {
            if (!strncmp(pszMimeType, RM_PARSER_LOGICAL_PREFIX,
                        strlen(RM_PARSER_LOGICAL_PREFIX)))
            {
                bRet = TRUE;
            }
        }
    }

    return bRet;
}

HXBOOL rm_parseri_is_logical_fileinfo_mime_type(const char* pszMimeType)
{
    HXBOOL bRet = FALSE;

    if (pszMimeType)
    {
        if (!strcmp(pszMimeType, RM_PARSER_LOGICAL_FILEINFO))
        {
            bRet = TRUE;
        }
    }

    return bRet;
}

HX_RESULT rm_parseri_read_next_header(rm_parser_internal* pInt, UINT32* pulID)
{
    HX_RESULT retVal = HXR_UNEXPECTED;

    if (pInt && pulID)
    {
        struct rm_generic_hdr genHdr;
        UINT32 ulHeaderSize   = 0;
        UINT32 ulBytesToRead  = 0;
        UINT32 ulNumBytesRead = 0;
        /* Set the return value */
        retVal = HXR_READ_ERROR;
        /* Read a generic header into the read buffer */
        ulBytesToRead  = RM_PARSER_GENERIC_HDR_SIZE;
        ulNumBytesRead = rm_parseri_file_read(pInt, ulBytesToRead, 0);
        /* Make sure we read enough */
        if (ulNumBytesRead == ulBytesToRead)
        {
            /* Unpack a generic header */
            rm_parseri_unpack_generic_hdr(pInt, &genHdr);
            /*
             * Compute the size of the header. For
             * most header types, this will be the size specified
             * in the "size" field. However, for data chunk headers and
             * index chunk headers, the "size" also includes the CONTENTS
             * of the chunk (all the packets or all the index records).
             * So for those chunks we must special case them.
             */
            ulHeaderSize  = genHdr.size;
            if (genHdr.id == RM_DATA_OBJECT)
            {
                ulHeaderSize = RM_PARSER_DATA_CHUNK_HEADER_SIZE;
            }
            else if (genHdr.id == RM_INDEX_OBJECT)
            {
                ulHeaderSize = RM_PARSER_INDEX_HEADER_SIZE;
            }
            /* Now compute how many bytes we still need to read */
            ulBytesToRead = ulHeaderSize - RM_PARSER_GENERIC_HDR_SIZE;
            /* Read the rest of the chunk */
            ulNumBytesRead = rm_parseri_file_read(pInt, ulBytesToRead, RM_PARSER_GENERIC_HDR_SIZE);
            if (ulNumBytesRead == ulBytesToRead)
            {
                /* Assign the header id to the out parameter */
                *pulID = genHdr.id;
                /* Clear the return value */
                retVal = HXR_OK;
            }
            else
            {
                /* Call back to error interface */
                rm_parseri_error(pInt, retVal, "Could not read chunk data.");
            }
        }
        else
        {
            /* Call back to error interface */
            rm_parseri_error(pInt, retVal, "Could not read chunk header.");
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_read_next_packet_header(rm_parser_internal* pInt,
                                             struct rm_pkt_hdr*  pPktHdr)
{
    HX_RESULT retVal = HXR_UNEXPECTED;

    if (pInt)
    {
        UINT32 ulNumBytesRead = 0;
        /* Change the error return */
        retVal = HXR_READ_ERROR;
        /* Read the packet header into the read buffer */
        ulNumBytesRead = rm_parseri_file_read(pInt, RM_PARSER_PACKET_HEADER_SIZE, 0);
        if (ulNumBytesRead == RM_PARSER_PACKET_HEADER_SIZE)
        {
            /* Unpack the packet header */
            retVal = rm_parseri_unpack_pkt_hdr(pInt, pPktHdr);
        }
        else
        {
            /* Call back to error interface */
            rm_parseri_error(pInt, retVal, "Could not read packet header.");
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_read_next_packet_header_buffer(rm_parser_internal* pInt,
                                             struct rm_pkt_hdr*  pPktHdr)
{
    HX_RESULT retVal = HXR_UNEXPECTED;
    HX_RESULT rv = HXR_OUTOFMEMORY;

    if (pInt)
    {
        UINT32 ulNumBytesRead = 0;
        /* Change the error return */
        retVal = HXR_READ_ERROR;
        /* Make sure our read buffer is big enough */
        rv = rm_enforce_buffer_min_size(pInt->pUserMem,
                                                  pInt->fpMalloc,
                                                  pInt->fpFree,
                                                  &pInt->pReadBuffer,
                                                  &pInt->ulReadBufferSize,
                                                  RM_PARSER_PACKET_HEADER_SIZE);
        if(rv == HXR_OK)
        {
            /* Read the packet header into the read buffer */
            ulNumBytesRead = rm_parseri_file_read_buffer(pInt, pInt->pReadBuffer, RM_PARSER_PACKET_HEADER_SIZE);
            pInt->ulNumBytesRead = ulNumBytesRead;
            //rm_parseri_error(pInt, ulNumBytesRead, "ulNumBytesRead");
            if(ulNumBytesRead == RM_PARSER_PACKET_HEADER_SIZE)
            {
                /* Unpack the packet header */
                retVal = rm_parseri_unpack_pkt_hdr(pInt, pPktHdr);
                //rm_parseri_error(pInt, pPktHdr->timestamp, "pPktHdr->timestamp");
            }
            else
            {
                rm_parseri_error(pInt, retVal, "Could not read packet header, read file error");
            }
        }
        else
        {
            /* Call back to error interface */
            rm_parseri_error(pInt, rv, "Could not read packet header,out of memory");
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_setup_interleaved_streams(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_NOT_SUPPORTED;

    if (pInt && pInt->pMediaPropsHdr && pInt->ulNumMediaPropsHdrs)
    {
        /* Declare some locals we'll need later */
        struct rm_media_props_hdr* pTmp = HXNULL;
        UINT32 ulNumInterleaved = 0;
        /* Allocate a temporary array of HXBOOLs */
        UINT32  ulSize       = pInt->ulNumMediaPropsHdrs * sizeof(HXBOOL);
        HXBOOL* pInterleaved = (HXBOOL*) rm_parseri_malloc(pInt, ulSize);
        if (pInterleaved)
        {
            /* Init them all the TRUE */
            UINT32 i = 0;
            UINT32 j = 0;
            for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
            {
                pInterleaved[i] = TRUE;
            }
            /*
             * Now run through the all the logical streams
             * and remove the physical streams
             */
            for (i = 0; i < pInt->ulNumLogicalStreamHdrs; i++)
            {
                for (j = 0; j < (UINT32) pInt->pLogicalStreamHdr[i].num_physical_streams; j++)
                {
                    pInterleaved[pInt->pLogicalStreamHdr[i].physical_stream_num[j]] = FALSE;
                }
            }
            /*
             * Now remove the media props headers that
             * are actually logical stream and logical fileinfo headers.
             */
            for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
            {
                const char* pszMimeType = (const char*) pInt->pMediaPropsHdr[i].mime_type;
                if (rm_parseri_is_logical_stream_mime_type(pszMimeType) ||
                    rm_parseri_is_logical_fileinfo_mime_type(pszMimeType))
                {
                    pInterleaved[i] = FALSE;
                }
            }
            /* Now count the interleaved streams */
            ulNumInterleaved = 0;
            for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
            {
                if (pInterleaved[i])
                {
                    ulNumInterleaved++;
                }
            }
            /* Do we have any interleaved streams? */
            if (ulNumInterleaved)
            {
                /* Allocate a new array of media props headers */
                retVal = HXR_OUTOFMEMORY;
                ulSize = ulNumInterleaved * sizeof(struct rm_media_props_hdr);
                pTmp   = (struct rm_media_props_hdr*) rm_parseri_malloc(pInt, ulSize);
                if (pTmp)
                {
                    /* Copy the media props headers which are interleaved */
                    j = 0;
                    for (i = 0; i < pInt->ulNumMediaPropsHdrs; i++)
                    {
                        if (pInterleaved[i])
                        {
                            pTmp[j++] = pInt->pMediaPropsHdr[i];
                        }
                        else
                        {
                            // free the Media Props Hdr.
                            rm_parseri_cleanup_media_props_hdr(pInt,&(pInt->pMediaPropsHdr[i]));
                        }
                    }
                    /* Free the logical stream array. */
                    rm_parseri_cleanup_all_logical_stream_hdrs(pInt);
                    /* Delete the memory associated with the old media props array */
                    rm_parseri_free(pInt, pInt->pMediaPropsHdr);
                    /* Replace the old media props array with the new one */
                    pInt->pMediaPropsHdr = pTmp;
                    /* Update the number of media props headers */
                    pInt->ulNumMediaPropsHdrs      = ulNumInterleaved;
                    pInt->ulNumMediaPropsHdrsAlloc = ulNumInterleaved;
                    /* Update the number of streams */
                    pInt->ulNumStreams = ulNumInterleaved;
                    /* Clear the return value */
                    retVal = HXR_OK;
                }
            }
        }
        /* Free the temporary array */
        rm_parseri_free(pInt, pInterleaved);
    }

    return retVal;
}

HX_RESULT rm_parseri_read_all_headers(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Declare the chunk id local variable */
        UINT32 ulID = 0;
        /* Seek back to the beginning of the file */
        rm_parseri_file_seek(pInt, 0, HX_SEEK_ORIGIN_SET);
        /* Read the first header */
        retVal = rm_parseri_read_next_header(pInt, &ulID);
        if (retVal == HXR_OK)
        {
            /*
             * If we don't encounter a RM_HEADER_OBJECT at the 
             * beginning of the file, then that's a problem.
             */
            if (ulID == RM_HEADER_OBJECT)
            {
                /* Unpack the .rm file header */
                retVal = rm_parseri_unpack_file_hdr(pInt);
                if (retVal == HXR_OK)
                {
                    /*
                     * Check the version of this file header struct
                     * as well as the file_version to make sure we
                     * support this version of the .rm file.
                     */
                    if ((pInt->fileHdr.version      == 0 || pInt->fileHdr.version      == 1) &&
                        (pInt->fileHdr.file_version == 0 || pInt->fileHdr.file_version == 1))
                    {
                        /*
                         * Now loop and unpack until we have either unpacked
                         * the number of headers specified in the file header
                         * or we reach the first data chunk.
                         */
                        HXBOOL bDone = FALSE;
                        UINT32 i     = 0;
                        for (i = 0; i < pInt->fileHdr.num_headers && !bDone && retVal == HXR_OK; i++)
                        {
                            /* Read the next header into the read buffer */
                            retVal = rm_parseri_read_next_header(pInt, &ulID);
                            if (retVal == HXR_OK)
                            {
                                /* Switch based on chunk 4cc */
                                switch (ulID)
                                {
                                    case RM_PROPERTIES_OBJECT:
                                        {
                                            /* Unpack the clip properties header */
                                            retVal = rm_parseri_unpack_properties_hdr(pInt);
                                            if (retVal == HXR_OK)
                                            {
                                                /* Allocate the media props array */
                                                retVal = rm_parseri_allocate_media_props_hdrs(pInt);
                                                if (retVal == HXR_OK)
                                                {
                                                    /* Init the number of media props headers we've seen */
                                                    pInt->ulNumMediaPropsHdrs = 0;
                                                }
                                            }
                                        }
                                        break;
                                    case RM_MEDIA_PROPERTIES_OBJECT:
                                        {
                                            /*
                                             * Media properties headers aren't necessarily
                                             * ordered in the file by stream_num. However, we would like
                                             * be able to index them by stream_num. Therefore, we will
                                             * first parse out the stream_num member and then place
                                             * it in the array at that index.
                                             */
                                            UINT32 ulStreamNum = rm_parseri_get_media_props_hdr_stream_num(pInt);
                                            if (ulStreamNum < pInt->ulNumMediaPropsHdrsAlloc)
                                            {
                                                /* Unpack the media properties header */
                                                retVal = rm_parseri_unpack_media_props_hdr(pInt, &pInt->pMediaPropsHdr[ulStreamNum]);
                                                /* Increment the number of properties we've read */
                                                if (retVal == HXR_OK)
                                                {
                                                    /* Increment the number of media props headers we've seen */
                                                    pInt->ulNumMediaPropsHdrs++;
                                                }
                                                else
                                                {
                                                    /* Parsing of media props header failed. */
                                                    retVal = HXR_CORRUPT_FILE;
                                                    /* Call back to the error interface */
                                                    rm_parseri_error(pInt, retVal, "Failed to parse media props header - possibly a corrupt file.");
                                                }
                                            }
                                            else
                                            {
                                                /* Illegal stream number */
                                                retVal = HXR_CORRUPT_FILE;
                                                /* Call back to the error interface */
                                                rm_parseri_error(pInt, retVal, "Illegal stream number - possibly a corrupt file.");
                                            }
                                        }
                                        break;
                                    case RM_CONTENT_OBJECT:
                                        {
                                            retVal = rm_parseri_unpack_content_hdr(pInt);
                                            if (retVal != HXR_OK)
                                            {
                                                /* Call back to the error interface */
                                                rm_parseri_error(pInt, retVal,
                                                                 "Failed to parse content header - possibly a corrupt file.");
                                            }
                                        }
                                        break;
                                    case RM_DATA_OBJECT:
                                        {
                                            bDone = TRUE;
                                        }
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        if (retVal == HXR_OK)
                        {
                            /* Parse all the logical stream headers */
                            retVal = rm_parseri_unpack_all_logical_stream_hdrs(pInt);
                            if (retVal == HXR_OK)
                            {
                                /*
                                * Currently we only support single-rate files and
                                * SureStream files who have a backward-compatible
                                * single-rate section. So know we must determine
                                * if we can play this file.
                                */
                                retVal = rm_parseri_setup_interleaved_streams(pInt);
                                if (retVal == HXR_OK)
                                {
                                    /* Create some structures to hold stream info */
                                    retVal = rm_parseri_create_stream_structures(pInt);
                                    if (retVal == HXR_OK)
                                    {
                                        /*
                                         * We will examine the first few packets
                                         * of each stream to determine if there is
                                         * a packet time offset.
                                         */
                                        retVal = rm_parseri_examine_initial_packets(pInt);
                                        if (retVal == HXR_OK)
                                        {
                                            /* Create the stream headers */
                                            retVal = rm_parseri_create_all_stream_headers(pInt);
                                        }
                                    }
                                }
                                else
                                {
                                    /*
                                     * This is a surestream file which has not backwards-compatible
                                     * streams, which we do not support.
                                     */
                                    retVal = HXR_NOT_SUPPORTED;
                                    /* Call back to the error interface */
                                    rm_parseri_error(pInt, retVal,
                                                     "Unsupported file (SureStream file with no "
                                                     "backwards-compatible streams)");
                                }
                            }
                        }
                    }
                    else
                    {
                        /*
                         * This is a later version than we support */
                        retVal = HXR_NOT_SUPPORTED;
                        /* Call back to the error interface */
                        rm_parseri_error(pInt, retVal, "Unsupported file header version.");
                    }
                }
            }
            else
            {
                /*
                 * The first four bytes of the file were
                 * not what we expected. This is likely not
                 * an .rm file.
                 * XXXMEH: is there a better error code for this?
                 */
                retVal = HXR_NOT_SUPPORTED;
                /* Call back to the error interface */
                rm_parseri_error(pInt, retVal,
                                 "First four bytes are are not \".RMF\" - this is not an .rm file");
            }
        }
    }

    return retVal;
}

UINT32 rm_parseri_get_num_file_properties(rm_parser_internal* pInt)
{
    UINT32 ulRet = 0;

    if (pInt)
    {
        /*
         * We always put the "StreamCount" and "Flags" properties.
         */
        ulRet += 2;
        /* If we have a real datatype, then we will add "IsRealDataType" */
        if (rm_parseri_has_real_data_type(pInt))
        {
            ulRet++;
        }
        /*
         * If we have a logical-fileinfo header, we will put in
         * all the properties in it.
         */
        if (pInt->pLogicalFileInfo)
        {
            ulRet += pInt->pLogicalFileInfo->num_props;
        }
        /* Add "Title", "Author", "Copyright" if present */
        if (pInt->contHdr.title)
        {
            ulRet++;
        }
        if (pInt->contHdr.author)
        {
            ulRet++;
        }
        if (pInt->contHdr.copyright)
        {
            ulRet++;
        }
    }

    return ulRet;
}

HX_RESULT rm_parseri_get_file_properties(rm_parser_internal* pInt,
                                         rm_property* pProp, UINT32 ulNumProp)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pProp && ulNumProp)
    {
        /* Init the counter */
        UINT32 ulIndex = 0;
        /* "StreamCount" property */
        retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++], "StreamCount",
                                            RM_PROPERTY_TYPE_UINT32,
                                            (void*) pInt->ulNumStreams, 0);

        if (retVal == HXR_OK)
        {
            retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++], "Flags",
                                                RM_PROPERTY_TYPE_UINT32,
                                                (void*) pInt->propHdr.flags, 0);
        }
        if (retVal == HXR_OK &&
            rm_parseri_has_real_data_type(pInt))
        {
            retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++], "IsRealDataType",
                                                RM_PROPERTY_TYPE_UINT32,
                                                (void*) 1, 0);
        }
        if (retVal == HXR_OK && pInt->contHdr.title)
        {
            retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++], "Title",
                                                RM_PROPERTY_TYPE_BUFFER,
                                                (void*) pInt->contHdr.title,
                                                strlen(pInt->contHdr.title) + 1);
        }
        if (retVal == HXR_OK && pInt->contHdr.author)
        {
            retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++], "Author",
                                                RM_PROPERTY_TYPE_BUFFER,
                                                (void*) pInt->contHdr.author,
                                                strlen(pInt->contHdr.author) + 1);
        }
        if (retVal == HXR_OK && pInt->contHdr.copyright)
        {
            retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++], "Copyright",
                                                RM_PROPERTY_TYPE_BUFFER,
                                                (void*) pInt->contHdr.copyright,
                                                strlen(pInt->contHdr.copyright) + 1);
        }
        if (pInt->pLogicalFileInfo)
        {
            UINT32 i = 0;
            for (i = 0; i < (UINT32) pInt->pLogicalFileInfo->num_props && retVal == HXR_OK; i++)
            {
                struct rm_name_value_map* pMap = &pInt->pLogicalFileInfo->props[i];
                UINT32 ulLength = pMap->value_sz;
                if (pMap->type != RM_PROPERTY_TYPE_BUFFER)
                {
                    ulLength = 0;
                }
                retVal = rm_parseri_set_rm_property(pInt, &pProp[ulIndex++],
                                                    (const char*) pMap->name,
                                                    pMap->type, pMap->value, ulLength);
            }
        }
    }

    return retVal;
}

HXBOOL rm_parseri_has_real_data_type(rm_parser_internal* pInt)
{
    HXBOOL bRet = FALSE;

    if (pInt)
    {
        bRet = pInt->bIsRealDataType;
    }

    return bRet;
}

HX_RESULT rm_parseri_set_rm_property(rm_parser_internal* pInt, rm_property* pProp,
                                     const char* pszName, UINT32 ulType,
                                     void* pValue, UINT32 ulValueLen)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pProp && pszName)
    {
        /* Cleanup the existing property */
        rm_parseri_cleanup_rm_property(pInt, pProp);
        /* Allocate space for the name */
        pProp->pName = (char*) rm_parseri_malloc(pInt, strlen(pszName) + 1);
        if (pProp->pName)
        {
            /* Copy the string */
            strcpy(pProp->pName, pszName);
            /* Copy the type */
            pProp->ulType = ulType;
            /* What type is this property? */
            switch (ulType)
            {
                case RM_PROPERTY_TYPE_UINT32:
                    {
                        pProp->pValue     = pValue;
                        pProp->ulValueLen = 0;
                        retVal            = HXR_OK;
                    }
                    break;
                case RM_PROPERTY_TYPE_BUFFER:
                    {
                        pProp->pValue = (BYTE*) rm_parseri_malloc(pInt, ulValueLen);
                        if (pProp->pValue)
                        {
                            /* Copy the string */
                            memcpy(pProp->pValue, pValue, ulValueLen);
                            /* Assign the length */
                            pProp->ulValueLen = ulValueLen;
                            /* Clear the return value */
                            retVal = HXR_OK;
                        }
                    }
                    break;
                case RM_PROPERTY_TYPE_CSTRING:
                    {
                        const char* pszValue = (const char*) pValue;
                        UINT32      ulLen    = (UINT32) strlen(pszValue) + 1;
                        pProp->pValue = (BYTE*) rm_parseri_malloc(pInt, ulLen);
                        if (pProp->pValue)
                        {
                            /* Copy the string */
                            strcpy((char*) pProp->pValue, pszValue);
                            /* Assign the length */
                            pProp->ulValueLen = ulLen;
                            /* Clear the return value */
                            retVal = HXR_OK;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return retVal;
}

void rm_parseri_cleanup_rm_property(rm_parser_internal* pInt, rm_property* pProp)
{
    if (pInt && pProp)
    {
        /* Free the name string */
        if (pProp->pName)
        {
            rm_parseri_free(pInt, pProp->pName);
            pProp->pName = HXNULL;
        }
        /*
         * If this is a RM_PROPERTY_TYPE_BUFFER or RM_PROPERTY_TYPE_CSTRING
         * property, then the pValue is an allocated buffer. If this
         * is an RM_PROPERTY_TYPE_UINT32 property, then pValue doesn't 
         * have any buffer associated with it.
         */
        if (pProp->pValue && pProp->ulType != RM_PROPERTY_TYPE_UINT32)
        {
            rm_parseri_free(pInt, pProp->pValue);
            pProp->pValue = HXNULL;
        }
    }
}

void rm_parseri_cleanup_stream_num_map(rm_parser_internal* pInt)
{
    if (pInt && pInt->pulStreamNumMap)
    {
        rm_parseri_free(pInt, pInt->pulStreamNumMap);
        pInt->pulStreamNumMap    = HXNULL;
        pInt->ulStreamNumMapSize = 0;
    }
}

void rm_parseri_cleanup_stream_info(rm_parser_internal* pInt, struct rm_stream_info* pInfo)
{
    if (pInt)
    {
        /* Free the index entry array */
        if (pInfo->seekTable.pEntry)
        {
            rm_parseri_free(pInt, pInfo->seekTable.pEntry);
            pInfo->seekTable.pEntry = HXNULL;
        }
    }
}

void rm_parseri_cleanup_stream_info_array(rm_parser_internal* pInt)
{
    if (pInt && pInt->pStreamInfo)
    {
        /* Clean up each individual stream info */
        UINT32 i = 0;
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            rm_parseri_cleanup_stream_info(pInt, &pInt->pStreamInfo[i]);
        }
        /* Clean up the array */
        rm_parseri_free(pInt, pInt->pStreamInfo);
        pInt->pStreamInfo = HXNULL;
    }
}

HX_RESULT rm_parseri_create_stream_structures(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->ulNumStreams)
    {
        /*
         * Compute the max stream number and the 
         * max duration across all streams.
         */
        UINT32 i                      = 0;
        UINT32 ulSize                 = 0;
        UINT32 ulMaxStreamNum         = pInt->pMediaPropsHdr[0].stream_num;
        UINT32 ulMaxDuration          = pInt->pMediaPropsHdr[0].duration;
        UINT32 ulIndexMaxEntries      = RM_INDEX_MAX_ENTRIES;
        UINT32 ulIndexTimeGranularity = ulMaxDuration / ulIndexMaxEntries;
        for (i = 1; i < pInt->ulNumStreams; i++)
        {
            if (pInt->pMediaPropsHdr[i].stream_num > ulMaxStreamNum)
            {
                ulMaxStreamNum = pInt->pMediaPropsHdr[i].stream_num;
            }
            if (pInt->pMediaPropsHdr[i].duration > ulMaxDuration)
            {
                ulMaxDuration = pInt->pMediaPropsHdr[i].duration;
            }
        }
        /* Assign the max duration */
        pInt->ulMaxDuration = ulMaxDuration;
        /* Free any existing stream number map */
        rm_parseri_cleanup_stream_num_map(pInt);
        /* Allocate the stream number map */
        pInt->ulStreamNumMapSize = ulMaxStreamNum + 1;
        ulSize                   = pInt->ulStreamNumMapSize * sizeof(UINT32);
        pInt->pulStreamNumMap = (UINT32*) rm_parseri_malloc(pInt, ulSize);
        if (pInt->pulStreamNumMap)
        {
            /* Initialize all the stream numbers in the map */
            for (i = 0; i < pInt->ulStreamNumMapSize; i++)
            {
                pInt->pulStreamNumMap[i] = RM_NO_STREAM_SET;
            }
            /*
             * The stream number maps maps stream number to 
             * the index in the pMediaPropsHdr array.
             */
            for (i = 0; i < pInt->ulNumStreams; i++)
            {
                pInt->pulStreamNumMap[pInt->pMediaPropsHdr[i].stream_num] = i;
            }
            /* Clean up any existing stream info array */
            rm_parseri_cleanup_stream_info_array(pInt);
            /* Allocate a stream info array */
            ulSize = pInt->ulNumStreams * sizeof(struct rm_stream_info);
            pInt->pStreamInfo = (struct rm_stream_info*) rm_parseri_malloc(pInt, ulSize);
            if (pInt->pStreamInfo)
            {
                /* Zero out the array */
                memset(pInt->pStreamInfo, 0, ulSize);
                /* Clear the return value */
                retVal = HXR_OK;
                /* Compute the index table parameters */
                if (ulIndexTimeGranularity > RM_INDEX_MAX_TIME_GRANULARITY)
                {
                    ulIndexTimeGranularity = RM_INDEX_MAX_TIME_GRANULARITY;
                    ulIndexMaxEntries      = ulMaxDuration / ulIndexTimeGranularity;
                    if (ulIndexMaxEntries > RM_INDEX_MAX_ENTRIES)
                    {
                        ulIndexMaxEntries = RM_INDEX_MAX_ENTRIES;
                    }
                }
                else if (ulIndexTimeGranularity < RM_INDEX_MIN_TIME_GRANULARITY)
                {
                    ulIndexTimeGranularity = RM_INDEX_MIN_TIME_GRANULARITY;
                    ulIndexMaxEntries      = ulMaxDuration / ulIndexTimeGranularity;
                    if (ulIndexMaxEntries < RM_INDEX_MIN_ENTRIES)
                    {
                        ulIndexMaxEntries = RM_INDEX_MIN_ENTRIES;
                    }
                }
                ulSize = ulIndexMaxEntries * sizeof(struct rm_seek_table_entry);
                /* Initialize the stream info */
                for (i = 0; i < pInt->ulNumStreams && retVal == HXR_OK; i++)
                {
                    /* Initialize the type flag */
                    if (pInt->pMediaPropsHdr[i].mime_type)
                    {
                        const char* pszStr = (const char*) pInt->pMediaPropsHdr[i].mime_type;
                        if (rm_stream_is_realaudio_mimetype(pszStr))
                        {
                            pInt->pStreamInfo[i].bIsRealAudio = TRUE;
                            pInt->bIsRealDataType             = TRUE;
                        }
                        else if (rm_stream_is_realvideo_mimetype(pszStr))
                        {
                            pInt->pStreamInfo[i].bIsRealVideo = TRUE;
                            pInt->bIsRealDataType             = TRUE;
                        }
                        else if (rm_stream_is_realevent_mimetype(pszStr))
                        {
                            pInt->pStreamInfo[i].bIsRealEvent = TRUE;
                            pInt->bIsRealDataType             = TRUE;
                        }
                    }
                    /* Initialize the index table */
                    pInt->pStreamInfo[i].seekTable.pEntry =
                        (struct rm_seek_table_entry*) rm_parseri_malloc(pInt, ulSize);
                    if (pInt->pStreamInfo[i].seekTable.pEntry)
                    {                    
                        /* Zero out the memory */
                        memset(pInt->pStreamInfo[i].seekTable.pEntry, 0, ulSize);
                        /* Init the parameters */
                        pInt->pStreamInfo[i].seekTable.ulMaxEntries      = ulIndexMaxEntries;
                        pInt->pStreamInfo[i].seekTable.ulTimeGranularity = ulIndexTimeGranularity;
                        pInt->pStreamInfo[i].seekTable.ulNumEntries      = 0;
                        pInt->pStreamInfo[i].seekTable.ulLastTime        = 0;
                        pInt->pStreamInfo[i].seekTable.ulRangeTime       = 0;
                    }
                    else
                    {
                        retVal = HXR_OUTOFMEMORY;
                    }
                }
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_examine_initial_packets(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        struct rm_pkt_hdr hdr;
        UINT32 ulID = 0;
        UINT32 i    = 0;
        UINT32 j    = 0;
        /* Seek to the data offset mentioned in the properties header */
        rm_parseri_file_seek(pInt, pInt->propHdr.data_offset, HX_SEEK_ORIGIN_SET);
        /*
         * Read the next header into the read buffer,
         * which should be a data chunk
         */
        retVal = rm_parseri_read_next_header(pInt, &ulID);
        if (retVal == HXR_OK)
        {
            /* Make sure it's a data chunk */
            if (ulID == RM_DATA_OBJECT)
            {
                /* Parse the data chunk header */
                retVal = rm_parseri_unpack_data_hdr(pInt);
                if (retVal == HXR_OK)
                {
                    /* Initialize the minimum first packet timestamp */
                    pInt->ulMinFirstPacketTime = 0xFFFFFFFF;
                    /*
                     * Now we will look at the first few packet headers
                     * until either we have seen at least one packet
                     * of each stream or we have verified that the 
                     * minimum timestamp across all streams is 0.
                     * We will loop as though we will look at all
                     * packets, but we will break out long before that.
                     */
                    for (i = 0; i < pInt->dataHdr.num_pkts && retVal == HXR_OK; i++)
                    {
                        retVal = rm_parseri_read_next_packet_header(pInt, &hdr);
                        if (retVal == HXR_OK)
                        {
                            /* Translate the stream number */
                            UINT32 ulStreamNum = rm_parseri_translate_stream_number(pInt, hdr.stream_num);
                            if (ulStreamNum != RM_NO_STREAM_SET)
                            {
                                /* Save the first timestamp of each stream */
                                if (!pInt->pStreamInfo[ulStreamNum].bSeenFirstPacketTimeStamp)
                                {
                                    pInt->pStreamInfo[ulStreamNum].ulLastTimeStamp = hdr.timestamp;
                                    pInt->pStreamInfo[ulStreamNum].bSeenFirstPacketTimeStamp = TRUE;
                                    /* Update the min first timestamp */
                                    if (hdr.timestamp < pInt->ulMinFirstPacketTime)
                                    {
                                        pInt->ulMinFirstPacketTime = hdr.timestamp;
                                    }
                                }
                                /* Check if we've seen the first timestamp of all streams */
                                for (j = 0; j < pInt->ulNumStreams; j++)
                                {
                                    if (!pInt->pStreamInfo[ulStreamNum].bSeenFirstPacketTimeStamp)
                                    {
                                        break;
                                    }
                                }
                                /*
                                 * Stop if we've seen a packet from each stream
                                 * or if we've already seen a timestamp of 0.
                                 */
                                if (j == pInt->ulNumStreams || pInt->ulMinFirstPacketTime == 0)
                                {
                                    /* Re-init the last timestamp for each stream */
                                    for (j = 0; j < pInt->ulNumStreams; j++)
                                    {
                                        pInt->pStreamInfo[j].ulLastTimeStamp = 0xFFFFFFFF;
                                        pInt->pStreamInfo[j].ulLastRule      = 0;
                                    }
                                    /* Seek back to right after the data chunk header */
                                    rm_parseri_file_seek(pInt,
                                                         pInt->propHdr.data_offset +
                                                         RM_PARSER_DATA_CHUNK_HEADER_SIZE,
                                                         HX_SEEK_ORIGIN_SET);
                                    /* Break out of the packet loop */
                                    break;
                                }
                                else
                                {
                                    /*
                                     * We need to keep reading packets, so 
                                     * seek past the data of this packet.
                                     */
                                    rm_parseri_file_seek(pInt,
                                                         hdr.length - RM_PARSER_PACKET_HEADER_SIZE,
                                                         HX_SEEK_ORIGIN_CUR);
                                }
                            }
                            else
                            {
                                retVal = HXR_INVALID_FILE;
                            }
                        }
                    }
                }
            }
            else
            {
                retVal = HXR_INVALID_FILE;
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_unpack_data_hdr(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= RM_PARSER_DATA_CHUNK_HEADER_SIZE)
        {
            /* Unpack the rm_data_hdr */
            pInt->dataHdr.id            = rm_unpack32(&pBuf, &ulLen);
            pInt->dataHdr.size          = rm_unpack32(&pBuf, &ulLen);
            pInt->dataHdr.version       = rm_unpack16(&pBuf, &ulLen);
            pInt->dataHdr.num_pkts      = rm_unpack32(&pBuf, &ulLen);
            pInt->dataHdr.next_data_hdr = rm_unpack32(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_unpack_pkt_hdr(rm_parser_internal* pInt,
                                    struct rm_pkt_hdr*  pPktHdr)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= RM_PARSER_PACKET_HEADER_SIZE)
        {
            /* Unpack the rm_pkt_hdr */
            pPktHdr->version    = rm_unpack16(&pBuf, &ulLen);
            pPktHdr->length     = rm_unpack16(&pBuf, &ulLen);
            pPktHdr->stream_num = rm_unpack16(&pBuf, &ulLen);
            pPktHdr->timestamp  = rm_unpack32(&pBuf, &ulLen);
            pPktHdr->flags      = rm_unpack16(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_unpack_index_hdr(rm_parser_internal* pInt,
                                      struct rm_index_hdr* hdr)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && hdr)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= RM_PARSER_INDEX_HEADER_SIZE)
        {
            /* Unpack the rm_index_hdr */
            hdr->id             = rm_unpack32(&pBuf, &ulLen);
            hdr->size           = rm_unpack32(&pBuf, &ulLen);
            hdr->version        = rm_unpack16(&pBuf, &ulLen);
            hdr->num_recs       = rm_unpack32(&pBuf, &ulLen);
            hdr->stream_num     = rm_unpack16(&pBuf, &ulLen);
            hdr->next_index_hdr = rm_unpack32(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_unpack_index_rec(rm_parser_internal* pInt,
                                      struct rm_index_rec* rec)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && rec)
    {
        /* Assign temporary variables */
        BYTE*  pBuf  = pInt->pReadBuffer;
        UINT32 ulLen = pInt->ulNumBytesRead;
        if (ulLen >= RM_PARSER_INDEX_RECORD_SIZE)
        {
            /* Unpack the rm_index_rec */
            rec->version   = rm_unpack16(&pBuf, &ulLen);
            rec->timestamp = rm_unpack32(&pBuf, &ulLen);
            rec->offset    = rm_unpack32(&pBuf, &ulLen);
            rec->num_pkts  = rm_unpack32(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_read_next_index_rec(rm_parser_internal* pInt,
                                         struct rm_index_rec* rec)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && rec)
    {
        UINT32 ulNumBytesRead = 0;
        /* Change the error return */
        retVal = HXR_READ_ERROR;
        /* Read the packet header into the read buffer */
        ulNumBytesRead = rm_parseri_file_read(pInt, RM_PARSER_INDEX_RECORD_SIZE, 0);
        if (ulNumBytesRead == RM_PARSER_INDEX_RECORD_SIZE)
        {
            /* Unpack the packet header */
            retVal = rm_parseri_unpack_index_rec(pInt, rec);
        }
    }

    return retVal;
}

UINT32 rm_parseri_translate_stream_number(rm_parser_internal* pInt, UINT32 ulNum)
{
    UINT32 ulRet = RM_NO_STREAM_SET;

    if (pInt && pInt->pulStreamNumMap && ulNum < pInt->ulStreamNumMapSize)
    {
        ulRet = pInt->pulStreamNumMap[ulNum];
    }

    return ulRet;
}

HX_RESULT rm_parseri_create_all_stream_headers(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->ulNumStreams)
    {
        UINT32 i      = 0;
        UINT32 ulSize = 0;
        /* Clean up any existing stream headers */
        rm_parseri_cleanup_all_stream_headers(pInt);
        /* Allocate memory for the stream header array */
        ulSize = pInt->ulNumStreams * sizeof(rm_stream_header);
        pInt->pStreamHdr = (rm_stream_header*) rm_parseri_malloc(pInt, ulSize);
        if (pInt->pStreamHdr)
        {
            /* NULL out the memory */
            memset(pInt->pStreamHdr, 0, ulSize);
            /* Clear the return value */
            retVal = HXR_OK;
            /* Create all stream headers */
            for (i = 0; i < pInt->ulNumStreams && retVal == HXR_OK; i++)
            {
                retVal = rm_parseri_create_stream_header(pInt, i, &pInt->pStreamHdr[i]);
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_create_stream_header(rm_parser_internal* pInt, UINT32 i, rm_stream_header* hdr)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->pMediaPropsHdr && i < pInt->ulNumStreams && hdr)
    {
        UINT32 ulNumProps = 0;
        UINT32 ulSize     = 0;
        /*
         * Count the number of properties. Passing in 
         * HXNULL and 0 means just count and don't set
         * the properties
         */
        ulNumProps = rm_parseri_count_set_stream_header_props(pInt, i, hdr,
                                                              HXNULL, 0);
        if (ulNumProps)
        {
            /* Compute the size of the array */
            ulSize = ulNumProps * sizeof(rm_property);
            /* Allocate the array */
            hdr->pProperty = (rm_property*) rm_parseri_malloc(pInt, ulSize);
            if (hdr->pProperty)
            {
                /* NULL out the array */
                memset(hdr->pProperty, 0, ulSize);
                /* Set the number of properties */
                hdr->ulNumProperties = ulNumProps;
                /* Call again, but this time pass in array */
                rm_parseri_count_set_stream_header_props(pInt, i, hdr,
                                                         hdr->pProperty,
                                                         hdr->ulNumProperties);
                /*
                 * Now set the properties for which we
                 * have dedicated convenience members. These
                 * are also duplicated in the rm_property array.
                 */
                hdr->ulStreamNumber  = i;
                hdr->ulMaxBitRate    = pInt->pMediaPropsHdr[i].max_bit_rate;
                hdr->ulAvgBitRate    = pInt->pMediaPropsHdr[i].avg_bit_rate;
                hdr->ulMaxPacketSize = pInt->pMediaPropsHdr[i].max_pkt_size;
                hdr->ulAvgPacketSize = pInt->pMediaPropsHdr[i].avg_pkt_size;
                hdr->ulDuration      = pInt->pMediaPropsHdr[i].duration;
                hdr->ulPreroll       = pInt->pMediaPropsHdr[i].preroll;
                hdr->ulStartTime     = pInt->pMediaPropsHdr[i].start_time;
                hdr->pMimeType       = copy_string(pInt->pUserMem, pInt->fpMalloc,
                                                   (const char*) pInt->pMediaPropsHdr[i].mime_type);
                hdr->pStreamName     = copy_string(pInt->pUserMem, pInt->fpMalloc,
                                                   (const char*) pInt->pMediaPropsHdr[i].stream_name);
                hdr->pOpaqueData     = copy_buffer(pInt->pUserMem, pInt->fpMalloc,
                                                   pInt->pMediaPropsHdr[i].type_spec,
                                                   pInt->pMediaPropsHdr[i].type_spec_sz);
                hdr->ulOpaqueDataLen = pInt->pMediaPropsHdr[i].type_spec_sz;
                /* Clear the return value */
                retVal = HXR_OK;
            }
        }
    }

    return retVal;
}

void rm_parseri_cleanup_all_stream_headers(rm_parser_internal* pInt)
{
    if (pInt && pInt->pStreamHdr)
    {
        /* Free the memory associated with each stream header */
        UINT32 i = 0;
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            rm_parseri_cleanup_stream_header(pInt, &pInt->pStreamHdr[i]);
        }
        /* Free the memory for the array */
        rm_parseri_free(pInt, pInt->pStreamHdr);
        /* NULL out the pointer */
        pInt->pStreamHdr = HXNULL;
    }
}

void rm_parseri_cleanup_stream_header(rm_parser_internal* pInt, rm_stream_header* hdr)
{
    if (pInt && hdr)
    {
        /* Free the mime type string */
        if (hdr->pMimeType)
        {
            rm_parseri_free(pInt, hdr->pMimeType);
            hdr->pMimeType = HXNULL;
        }
        /* Free the stream name string */
        if (hdr->pStreamName)
        {
            rm_parseri_free(pInt, hdr->pStreamName);
            hdr->pStreamName = HXNULL;
        }
        /* Free the opaque data buffer */
        if (hdr->pOpaqueData)
        {
            rm_parseri_free(pInt, hdr->pOpaqueData);
            hdr->pOpaqueData     = HXNULL;
            hdr->ulOpaqueDataLen = 0;
        }
        /* Do we have any properties? */
        if (hdr->pProperty)
        {
            /* Clean up each individual rm_property */
            UINT32 i = 0;
            for (i = 0; i < hdr->ulNumProperties; i++)
            {
                rm_parseri_cleanup_rm_property(pInt, &hdr->pProperty[i]);
            }
            /* Free the memory for the property array */
            rm_parseri_free(pInt, hdr->pProperty);
            /* Null out the pointer */
            hdr->pProperty       = HXNULL;
            hdr->ulNumProperties = 0;
        }
    }
}

UINT32 rm_parseri_count_set_stream_header_props(rm_parser_internal* pInt, UINT32 i,
                                                rm_stream_header* hdr,
                                                rm_property* pProp, UINT32 ulNumProps)
{
    UINT32 ulRet = 0;

    if (pInt && i < pInt->ulNumStreams && hdr)
    {
        /* Initialize some local variables used later */
        UINT32   ulDuration         = pInt->pMediaPropsHdr[i].duration;
        UINT32   ulPreroll          = pInt->pMediaPropsHdr[i].preroll;
        UINT32   ulStreamingPreroll = 0;
        UINT32   ulOldPreroll       = 0;
        UINT32   ulPreDecBufSize    = 0;
        UINT32   ulPreDecBufTime    = 0;
        HXDOUBLE dPreDecBufSize     = 0.0;
        HXBOOL   bIsVBR             = (pInt->pMediaPropsHdr[i].max_bit_rate !=
                                       pInt->pMediaPropsHdr[i].avg_bit_rate ? TRUE : FALSE);
        HXBOOL   bHasPreDataProps   = FALSE;
        HXDOUBLE dPreData           = 0.0;
        UINT32   ulPreData          = 0;
        char*    pASMRuleBook       = HXNULL;
        BYTE*    pBuf               = HXNULL;
        UINT32   ulLen              = 0;
        BYTE     ucRuleToFlag[8];
        /* Set/count the "StreamNumber" property */
        if (pProp)
        {
            rm_parseri_set_rm_property(pInt, &pProp[ulRet], "StreamNumber",
                                       RM_PROPERTY_TYPE_UINT32, (void*) i, 0);
        }
        ulRet++;
        /* Set/count the "MaxBitRate" property */
        if (pInt->pMediaPropsHdr[i].max_bit_rate)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "MaxBitRate",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) pInt->pMediaPropsHdr[i].max_bit_rate, 0);
            }
            ulRet++;
        }
        /* Set/count the "AvgBitRate" property */
        if (pInt->pMediaPropsHdr[i].avg_bit_rate)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "AvgBitRate",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) pInt->pMediaPropsHdr[i].avg_bit_rate, 0);
            }
            ulRet++;
        }
        /* Set/count the "MaxPacketSize" property */
        if (pInt->pMediaPropsHdr[i].max_pkt_size)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "MaxPacketSize",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) pInt->pMediaPropsHdr[i].max_pkt_size, 0);
            }
            ulRet++;
        }
        /* Set/count the "AvgPacketSize" property */
        if (pInt->pMediaPropsHdr[i].avg_pkt_size)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "AvgPacketSize",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) pInt->pMediaPropsHdr[i].avg_pkt_size, 0);
            }
            ulRet++;
        }
        /* Set/count the "StartTime" property */
        if (pInt->pMediaPropsHdr[i].start_time)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "StartTime",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) pInt->pMediaPropsHdr[i].start_time, 0);
            }
            ulRet++;
        }
        /* Set/count the "MimeType" property */
        if (pInt->pMediaPropsHdr[i].mime_type)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "MimeType",
                                           RM_PROPERTY_TYPE_CSTRING,
                                           (void*) pInt->pMediaPropsHdr[i].mime_type, 0);
            }
            ulRet++;
        }
        /* Set/count the "StreamName" property */
        if (pInt->pMediaPropsHdr[i].stream_name)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "StreamName",
                                           RM_PROPERTY_TYPE_CSTRING,
                                           (void*) pInt->pMediaPropsHdr[i].stream_name, 0);
            }
            ulRet++;
        }
        /* Set/count the "OpaqueData" property */
        if (pInt->pMediaPropsHdr[i].type_spec)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "OpaqueData",
                                           RM_PROPERTY_TYPE_BUFFER,
                                           (void*) pInt->pMediaPropsHdr[i].type_spec,
                                           pInt->pMediaPropsHdr[i].type_spec_sz);
            }
            ulRet++;
        }
        /* Set/count the "intrinsicDurationType" property */
        if (pInt->pStreamInfo[i].bIsRealAudio ||
            pInt->pStreamInfo[i].bIsRealVideo ||
            pInt->pStreamInfo[i].bIsRealEvent)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "intrinsicDurationType",
                                           RM_PROPERTY_TYPE_CSTRING,
                                           (void*) "intrinsicDurationContinuous", 0);
            }
            ulRet++;
        }
        /* Set/count the "EndTime" property */
        if (pInt->pMediaPropsHdr[i].duration > pInt->propHdr.duration)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "EndTime",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) pInt->propHdr.duration, 0);
            }
            ulRet++;
        }
        /* Set/count the "EndOneRuleEndAll" property */
        if (pInt->pStreamInfo[i].bIsRealAudio ||
            pInt->pStreamInfo[i].bIsRealVideo)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "EndOneRuleEndAll",
                                           RM_PROPERTY_TYPE_UINT32, (void*) 1, 0);
            }
            ulRet++;
        }
        /* Set/count the "CanBeStoppedAnyTime" property */
        if (pInt->pStreamInfo[i].bIsRealEvent)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "CanBeStoppedAnyTime",
                                           RM_PROPERTY_TYPE_UINT32, (void*) 1, 0);
            }
            ulRet++;
        }
        /* Do some adjustments to duration */
        if (pInt->pStreamInfo[i].bIsRealEvent)
        {
            /*
             * If this is an event stream, then set the stream duration
             * to the maximum duration across all streams.
             */
            ulDuration = pInt->ulMaxDuration;
        }
        /* Subtract off the min first packet time */
        if (ulDuration > pInt->ulMinFirstPacketTime)
        {
            ulDuration -= pInt->ulMinFirstPacketTime;
        }
        /* Set/count the "Duration" property */
        if (pProp)
        {
            rm_parseri_set_rm_property(pInt, &pProp[ulRet], "Duration",
                                       RM_PROPERTY_TYPE_UINT32,
                                       (void*) ulDuration, 0);
        }
        ulRet++;
        /* Do some adjustments to preroll values */
        if (pInt->pStreamInfo[i].bIsRealAudio)
        {
            /* Save the preroll */
            ulOldPreroll = ulPreroll;
            /*
             * RealAudio streams need to have 2 superblocks as the preroll value.
             * Encoders put one superblock duration as the preroll.
             */
            ulPreroll  = (ulPreroll ? ulPreroll * 2 : 6000);
        }
        if (pInt->pStreamInfo[i].bIsRealVideo ||
            pInt->pStreamInfo[i].bIsRealAudio)
        {
            /* Compute the pre-decode buffer size */
            if (pInt->pStreamInfo[i].bIsRealVideo)
            {
                /* For VBR stream, the PreDecBufTime is the same as the actual preroll */
                if (bIsVBR)
                {
                    ulPreDecBufTime = ulPreroll;
                }
                /* 
                 * If actual preroll is less than 1 sec, set preroll
                 * and PreDecBufTime to 1 sec more than ActualPreroll
                 */
                if (ulPreroll < 1000)
                {
                    if (ulOldPreroll == 0)
                    {
                        ulOldPreroll = ulPreroll;
                    }
                    ulPreroll += 1000;
                    if (!bIsVBR)
                    {
                        ulPreDecBufTime = ulPreroll;
                    }
                }
                else if (!bIsVBR)
                {
                    /*
                     * Otherwise for CBR set the PreDecBufTime to
                     * twice the actual preroll
                     */
                    ulPreDecBufTime = ulPreroll * 2;
                }
                /* Now compute pre-decode buffer size, rounding up */
                dPreDecBufSize  = (((HXDOUBLE) ulPreDecBufTime) *
                                   ((HXDOUBLE) pInt->pMediaPropsHdr[i].max_bit_rate) +
                                   7999.0) / 8000.0;
                ulPreDecBufSize = (UINT32) dPreDecBufSize;
            }
            /*
             * Cap the preroll at 5000 ms.
             * XXXMEH - this assumes local playback
             */
            if (ulPreroll > 5000)
            {
                if (!ulOldPreroll)
                {
                    ulOldPreroll = ulPreroll;
                }
                ulStreamingPreroll = ulPreroll;
                ulPreroll          = 5000;
            }
        }
        /* Set/count the "Preroll" property */
        if (pProp)
        {
            rm_parseri_set_rm_property(pInt, &pProp[ulRet], "Preroll",
                                       RM_PROPERTY_TYPE_UINT32,
                                       (void*) ulPreroll, 0);
        }
        ulRet++;
        /* Set/count the "ActualPreroll" property */
        if (ulOldPreroll)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "ActualPreroll",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) ulOldPreroll, 0);
            }
            ulRet++;
        }
        /* Set/count the "StreamingPreroll" property */
        if (ulStreamingPreroll)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "StreamingPreroll",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) ulStreamingPreroll, 0);
            }
            ulRet++;
        }
        /* Set/count the "X-PreDecBufSize" property */
        if (ulPreDecBufSize)
        {
            if (pProp)
            {
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "X-PreDecBufSize",
                                           RM_PROPERTY_TYPE_UINT32,
                                           (void*) ulPreDecBufSize, 0);
            }
            ulRet++;
        }
        /* Create the ASM rule book string */
        pASMRuleBook = rm_parseri_create_asm_rulebook(pInt,
                                                      pInt->pStreamInfo[i].bIsRealVideo,
                                                      pInt->pStreamInfo[i].bIsRealEvent,
                                                      (pInt->pMediaPropsHdr[i].type_spec_sz ? TRUE : FALSE),
                                                      pInt->pMediaPropsHdr[i].max_bit_rate,
                                                      pInt->pMediaPropsHdr[i].avg_bit_rate,
                                                      &bHasPreDataProps);
        if (pASMRuleBook)
        {
            /* Set/count the "ASMRuleBook" property */
            if (pProp)
            {
                /* Set it into the property */
                rm_parseri_set_rm_property(pInt, &pProp[ulRet], "ASMRuleBook",
                                           RM_PROPERTY_TYPE_CSTRING,
                                           (void*) pASMRuleBook, 0);
            }
            ulRet++;
        }
        /* Free the string */
        free_string(pInt->pUserMem, pInt->fpFree, &pASMRuleBook);
        /* Set/count the pre-data related properties */
        if (bHasPreDataProps)
        {
            dPreData  = ((HXDOUBLE) ulPreroll) *
                        ((HXDOUBLE) pInt->pMediaPropsHdr[i].max_bit_rate) /
                        8000.0;
            ulPreData = (UINT32) (dPreData + 0.5);
            if (ulPreData)
            {
                if (pProp)
                {
                    rm_parseri_set_rm_property(pInt, &pProp[ulRet], "PreData",
                                               RM_PROPERTY_TYPE_UINT32,
                                               (void*) ulPreData, 0);
                    rm_parseri_set_rm_property(pInt, &pProp[ulRet+1], "PreDataAtStart",
                                               RM_PROPERTY_TYPE_UINT32, (void*) 1, 0);
                    rm_parseri_set_rm_property(pInt, &pProp[ulRet+2], "PreDataAfterSeek",
                                               RM_PROPERTY_TYPE_UINT32, (void*) 1, 0);
                    rm_parseri_set_rm_property(pInt, &pProp[ulRet+3], "PrerollAfterSeek",
                                               RM_PROPERTY_TYPE_UINT32, (void*) 1, 0);
                }
                ulRet += 4;
            }
        }
        /* Set/count the "RMFF 1.0 Flags" property */
        if (pInt->pStreamInfo[i].bIsRealVideo ||
            pInt->pStreamInfo[i].bIsRealEvent ||
            pInt->pMediaPropsHdr[i].type_spec_sz)
        {
            if (pProp)
            {
                if (pInt->pStreamInfo[i].bIsRealVideo)
                {
                    /*
                     * For video streams, we will set 3 rules. Rule 0
                     * is a keyframe rule, rule 1 is a non-keyframe rule,
                     * and rule 2 is a keyframe (thinning) rule.
                     */
                    pBuf  = &ucRuleToFlag[0];
                    ulLen = 8;
                    rm_pack16(3,                &pBuf, &ulLen);
                    rm_pack16(HX_KEYFRAME_FLAG, &pBuf, &ulLen);
                    rm_pack16(0,                &pBuf, &ulLen);
                    rm_pack16(HX_KEYFRAME_FLAG, &pBuf, &ulLen);
                    /* Set the property */
                    rm_parseri_set_rm_property(pInt, &pProp[ulRet], "RMFF 1.0 Flags",
                                               RM_PROPERTY_TYPE_BUFFER,
                                               (void*) &ucRuleToFlag[0], 8);
                }
                else if (pInt->pStreamInfo[i].bIsRealEvent ||
                         pInt->pMediaPropsHdr[i].type_spec_sz)
                {
                    /*
                     * For non-video streams, we will set 2 rules. Rule 0
                     * is a keyframe rule, rule 1 is a non-keyframe rule.
                     */
                    pBuf  = &ucRuleToFlag[0];
                    ulLen = 8;
                    rm_pack16(2,                &pBuf, &ulLen);
                    rm_pack16(HX_KEYFRAME_FLAG, &pBuf, &ulLen);
                    rm_pack16(0,                &pBuf, &ulLen);
                    /* Set the property */
                    rm_parseri_set_rm_property(pInt, &pProp[ulRet], "RMFF 1.0 Flags",
                                               RM_PROPERTY_TYPE_BUFFER,
                                               (void*) &ucRuleToFlag[0], 6);
                }
            }
            ulRet++;
        }
    }

    return ulRet;
}

char* rm_parseri_create_asm_rulebook(rm_parser_internal* pInt, HXBOOL bIsRealVideo,
                                     HXBOOL bIsRealEvent, HXBOOL bHasOpaqueData,
                                     UINT32 ulMaxBitRate, UINT32 ulAvgBitRate,
                                     HXBOOL* pbHasPreDataProps)
{
    char* pRet = HXNULL;

    if (pInt && pbHasPreDataProps)
    {
        HXBOOL bIsVBR = (ulMaxBitRate != ulAvgBitRate ? TRUE : FALSE);
        /* Allocate the output buffer */
        pRet = (char*) rm_parseri_malloc(pInt, RM_PARSER_RULE_BOOK_BUFFER_SIZE);
        if (pRet)
        {
            /* Is this a RealVideo stream? */
            if (bIsRealVideo)
            {
                /* Is this VBR? */
                if (bIsVBR)
                {
                    if (ulMaxBitRate)
                    {
                        /* VBR RealVideo with max_bit_rate != 0 */
                        sprintf(pRet, g_pszRuleBook[0], 9, ulAvgBitRate, ulMaxBitRate, 5);
                    }
                    else
                    {
                        /* VBR RealVideo with max_bit_rate == 0 */
                        sprintf(pRet, g_pszRuleBook[1], 9, ulAvgBitRate, 5);
                    }
                    /* Set the PreData flag */
                    *pbHasPreDataProps = TRUE;
                }
                else
                {
                    /* CBR video */
                    sprintf(pRet, g_pszRuleBook[5], ulAvgBitRate, ulAvgBitRate,
                            ulAvgBitRate, ulAvgBitRate);
                }
            }
            else if (bIsRealEvent || bHasOpaqueData)
            {
                /* This is most likely an event or audio stream */
                if (bIsRealEvent)
                {
                    /* RealEvent stream */
                    sprintf(pRet, g_pszRuleBook[6]);
                }
                else if (bIsVBR)
                {
                    /* Most likely an audio stream */
                    if (ulMaxBitRate)
                    {
                        /* VBR RealAudio with max_bit_rate != 0 */
                        sprintf(pRet, g_pszRuleBook[0], 5, ulAvgBitRate, ulMaxBitRate, 5);
                    }
                    else
                    {
                        /* VBR RealAudio with max_bit_rate == 0 */
                        sprintf(pRet, g_pszRuleBook[1], 5, ulAvgBitRate, 5);
                    }
                    /* Set the PreData flag */
                    *pbHasPreDataProps = TRUE;
                }
                else
                {
                    /* CBR RealAudio */
                    sprintf(pRet, g_pszRuleBook[7], ulAvgBitRate);
                }
            }
            else
            {
                /* Non-RealAudio/RealVideo/RealEvent streams */
                if (bIsVBR)
                {
                    if (ulMaxBitRate)
                    {
                        sprintf(pRet, g_pszRuleBook[2], ulAvgBitRate, ulMaxBitRate);
                    }
                    else if (ulAvgBitRate)
                    {
                        sprintf(pRet, g_pszRuleBook[3], ulAvgBitRate);
                    }
                    else
                    {
                        sprintf(pRet, g_pszRuleBook[4]);
                    }
                }
                else
                {
                    /* CBR Non-RealAudio/RealVideo/RealEvent streams */
                    sprintf(pRet, g_pszRuleBook[8], ulAvgBitRate);
                }
            }
        }
    }

    return pRet;
}

HX_RESULT rm_parseri_read_next_packet(rm_parser_internal* pInt, rm_packet** ppPacket)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && ppPacket)
    {
        /* Make sure we're still in the range of the data chunk */
        if (pInt->ulCurFileOffset < pInt->propHdr.data_offset + pInt->dataHdr.size)
        {
            /* More packets to go - read the next one */
            struct rm_pkt_hdr hdr;
            rm_packet* pPacket          = HXNULL;
            UINT32     ulStreamNum      = 0;
            UINT32     ulASMRule        = 0;
            UINT32     ulASMFlags       = 0;
            UINT32     ulLastPacketTime = 0;
            UINT32     ulLastRule       = 0;
            UINT32     ulBytesRead      = 0;
            UINT32     ulDataSize       = 0;
            UINT32     ulPacketOffset   = 0;
            HXBOOL     bSkipPacket      = FALSE;
            do
            {
                /* Read packet header form pPacketBuffer */
                retVal = rm_parseri_read_next_packet_header_buffer(pInt, &hdr);
                if (retVal == HXR_OK)
                {
                    /* Assume the worst */
                    retVal = HXR_FAIL;
                    /* Translate the stream number */
                    ulStreamNum = rm_parseri_translate_stream_number(pInt, hdr.stream_num);
                    if (ulStreamNum != RM_NO_STREAM_SET)
                    {
                        /* Get the rm_stream_info for this stream */
                        struct rm_stream_info* pInfo = &pInt->pStreamInfo[ulStreamNum];
                        if (pInfo)  //lint  !e774
                        {
                            /* Clear the return value */
                            retVal = HXR_OK;
                            /* Are we currently looking for keyframes? */
                            if (pInt->ulKeyframesNeeded)
                            {
                                /*
                                 * Is this a keyframe, and do we need a keyframe
                                 * for this stream?
                                 */
                                if (!pInfo->bNeedKeyframe ||
                                    ((hdr.flags & HX_KEYFRAME_FLAG) &&
                                     (!pInfo->keyFramePacket.bValid ||
                                      hdr.timestamp >= pInfo->keyFramePacket.ulTimestamp)))
                                {
                                    if (pInfo->bNeedKeyframe)
                                    {
                                        pInfo->bNeedKeyframe = FALSE;
                                        pInt->ulKeyframesNeeded--;
                                    }
                                    bSkipPacket = FALSE;
                                }
                                else
                                {
                                    /*
                                     * We need keyframes for this stream and
                                     * this is not a keyframe.
                                     */
                                    bSkipPacket = TRUE;
                                    /* Seek past the data of this packet */
                                    if(hdr.length > RM_PARSER_PACKET_HEADER_SIZE)
                                    {
                                        rm_parseri_file_seek(pInt,
                                                             hdr.length - RM_PARSER_PACKET_HEADER_SIZE,
                                                             HX_SEEK_ORIGIN_CUR);
                                        /* Update  ulCurBufferOffset */
                                        pInt->ulCurBufferOffset += hdr.length - RM_PARSER_PACKET_HEADER_SIZE;
                                    }
                                    else
                                    {
                                        rm_parseri_error(pInt, hdr.length, "error hdr.length"); 
                                        retVal = HXR_CORRUPT_FILE;
                                    }
                                }
                            }
                            else
                            {
                                bSkipPacket = FALSE;
                            }
                        }
                    }
                    else
                    {
                        /* Got a stream number we didn't expect */
                        retVal = HXR_CORRUPT_FILE;
                        /* Call back to error interface */
                        rm_parseri_error(pInt, retVal, "Unexpected stream number in packet.");
                    }
                }
            }
            while (HX_SUCCEEDED(retVal) && bSkipPacket);
            /* Have we found a packet? */
            if (HX_SUCCEEDED(retVal))
            {
                /* Get the last packet time and rule for this stream */
                ulLastPacketTime = pInt->pStreamInfo[ulStreamNum].ulLastTimeStamp;
                ulLastRule       = pInt->pStreamInfo[ulStreamNum].ulLastRule;
                /* Determine the ASM rules and flags */
                if (pInt->pStreamInfo[ulStreamNum].bIsRealVideo)
                {
                    /*
                     * If the time stamp for this packet is different from
                     * the timestamp of the next packet and either this packet
                     * is a keyframe or the last packet was a keyframe we can
                     * switch on this packet
                     */
                    if (hdr.timestamp != ulLastPacketTime &&
                        (hdr.flags & HX_KEYFRAME_FLAG   ||
                         ulLastRule == HX_KEYFRAME_RULE ||
                         ulLastRule == HX_THINNING_RULE))
                    {
                        ulASMFlags = HX_ASM_SWITCH_ON | HX_ASM_SWITCH_OFF;
                    }
                    else if (hdr.timestamp != ulLastPacketTime)
                    {
                        /* If this is part of a different frame, we can switch off */
                        ulASMFlags = HX_ASM_SWITCH_OFF;
                    }
                    else
                    {
                        /* 
                         * If this packet has the same timestamp as the previous
                         * packet we cannot switch on or switch off here.
                         */
                        ulASMFlags = 0;
                    }
                    /* Assign the ASM rule */
                    ulASMRule = (hdr.flags & HX_KEYFRAME_FLAG ? HX_KEYFRAME_RULE : HX_NONKEYFRAME_RULE);
                }
                else if (pInt->pStreamInfo[ulStreamNum].bIsRealAudio)
                {
                    /*
                     * For audio, the first block of the super block and the second block
                     * of the super block are ON since the first block and the second block
                     * are the first packets on their respective rule numbers
                     */
	            if (hdr.flags & HX_KEYFRAME_FLAG || ulLastRule == HX_KEYFRAME_RULE)
	            {
                        ulASMFlags = HX_ASM_SWITCH_ON | HX_ASM_SWITCH_OFF;
	            }
	            else
	            {
                        ulASMFlags = HX_ASM_SWITCH_OFF;
	            }
                    /* Assign the ASM rule */
                    ulASMRule = (hdr.flags & HX_KEYFRAME_FLAG ? HX_KEYFRAME_RULE : HX_NONKEYFRAME_RULE);
                }
                else
                {
                    /* Assign the ASM flags */
                    ulASMFlags  = HX_ASM_SWITCH_OFF;
                    ulASMFlags |= (hdr.flags & HX_KEYFRAME_FLAG ? HX_ASM_SWITCH_ON : 0);
                    /* Is this an event stream or file version 0? */
                    if (pInt->pStreamInfo[ulStreamNum].bIsRealEvent ||
                        pInt->fileHdr.file_version == 0)
                    {
                        ulASMRule = (hdr.flags & HX_KEYFRAME_FLAG ? HX_KEYFRAME_RULE : HX_NONKEYFRAME_RULE);
                    }
                    else
                    {
                        /*
                         * In case of non-RM streams version 1 or later, the rule
                         * number is recorded directly into the upper 8 bits of the
                         * flags field (packet_group)
                         */
                        ulASMRule = (UINT32) ((hdr.flags >> 8) & 0x00FF);
                    }
                }
                /* Set the return value */
                retVal = HXR_OUTOFMEMORY;
                /* Allocate space for the rm_packet */
                pPacket = (rm_packet*) rm_parseri_malloc(pInt, sizeof(rm_packet));
                if (pPacket)
                {
                    /* NULL out the packet */
                    memset(pPacket, 0, sizeof(rm_packet));
                    /* Compute the data size */
                    ulDataSize = hdr.length - RM_PARSER_PACKET_HEADER_SIZE;
                    /* Allocate space for the data */
                    pPacket->pData = (BYTE*) rm_parseri_malloc(pInt, ulDataSize);
                    if (pPacket->pData)
                    {
                        /* Read the data from pPaceketBuffer into the buffer */
                        ulBytesRead = rm_parseri_file_read_buffer(pInt, pPacket->pData, ulDataSize);
                        //rm_parseri_error(pInt, ulDataSize, "ulDataSize");
                        //rm_parseri_error(pInt, ulBytesRead, "ulBytesRead");
                        if (ulBytesRead == ulDataSize)
                        {
                            /* Assign the packet parameters */
                            pPacket->ulTime     = hdr.timestamp;
                            pPacket->usStream   = (UINT16) ulStreamNum;
                            pPacket->usASMFlags = (UINT16) ulASMFlags;
                            pPacket->ucASMRule  = (BYTE) ulASMRule;
                            pPacket->ucLost     = FALSE;
                            pPacket->usDataLen  = (UINT16) ulDataSize;
                            /* Update the last timestamp and rule */
                            pInt->pStreamInfo[ulStreamNum].ulLastTimeStamp = hdr.timestamp;
                            pInt->pStreamInfo[ulStreamNum].ulLastRule      = ulASMRule;
                            /* Compute the file offset of this packet */
                            ulPacketOffset = pInt->ulCurFileOffset -
                                             ulDataSize - RM_PARSER_PACKET_HEADER_SIZE;
                            /* Update the seek table */
                            rm_parseri_update_seek_table(pInt,
                                                         ulStreamNum,
                                                         hdr.timestamp,
                                                         ulPacketOffset,
                                                         hdr.flags);
                            /* Assign the out parameter */
                            *ppPacket = pPacket;
                            /* Clear the return value */
                            retVal = HXR_OK;
                        }
                        else
                        {
                            /* Could not read packet data */
                            retVal = HXR_READ_ERROR;
                            /* Call back to error interface */
                            rm_parseri_error(pInt, retVal, "Could not read packet data.");
                        }
                    }
                    if (retVal != HXR_OK)
                    {
                        /* Free the packet data */
                        if (pPacket->pData)
                        {
                            rm_parseri_free(pInt, pPacket->pData);
                            pPacket->pData = HXNULL;
                        }
                        /* Free the packet */
                        rm_parseri_free(pInt, pPacket);
                        pPacket = HXNULL;
                    }
                }
            }
        }
        else
        {
            /* We are finished reading packets from this chunk */
            retVal = HXR_AT_END;
        }
    }

    return retVal;
}

UINT32 rm_parseri_file_read(rm_parser_internal* pInt, UINT32 ulBytesToRead, UINT32 ulReadBufferOffset)
{
    UINT32 ulRet = 0;

    if (pInt && pInt->fpRead)
    {
        /* Make sure our read buffer is big enough */
        HX_RESULT rv = rm_enforce_buffer_min_size(pInt->pUserMem,
                                                  pInt->fpMalloc,
                                                  pInt->fpFree,
                                                  &pInt->pReadBuffer,
                                                  &pInt->ulReadBufferSize,
                                                  ulReadBufferOffset + ulBytesToRead);
        if (rv == HXR_OK)
        {
            /* Read in ulBytesToRead bytes */
            ulRet = pInt->fpRead(pInt->pUserRead,
                                 pInt->pReadBuffer + ulReadBufferOffset,
                                 ulBytesToRead);
            /* Save the number of bytes in read buffer */
            pInt->ulNumBytesRead = ulRet + ulReadBufferOffset;
            /* Advance the current file offset */
            pInt->ulCurFileOffset += ulRet;
        }
    }

    return ulRet;
}

UINT32 rm_parseri_file_read_buffer(rm_parser_internal* pInt, BYTE* pBuf, UINT32 ulBytesToRead)
{
    UINT32 ulRet = 0;

    if (pInt && pInt->fpRead)
    {
        //rm_parseri_error(pInt, ulBytesToRead, "ulBytesToRead");
        //rm_parseri_error(pInt, pInt->ulCurBufferOffset, "ulCurBufferOffset");
        //rm_parseri_error(pInt, pInt->ulUsedBufferSize, "pInt->ulUsedBufferSize");
        /*  pPacketBuffer have not enough bytes to be read */
        if(pInt->ulCurBufferOffset +  ulBytesToRead > pInt->ulUsedBufferSize)
        {
            /* Read ulPacketBufferSize bytes from file to pPacketBuffer */
            pInt->fpSeek(pInt->pUserRead, pInt->ulCurFileOffset, HX_SEEK_ORIGIN_SET);
            ulRet = pInt->fpRead(pInt->pUserRead, pInt->pPacketBuffer, pInt->ulPacketBufferSize);
            pInt->ulUsedBufferSize = ulRet;
            pInt->ulCurBufferOffset = 0;
            if(ulRet < ulBytesToRead)
            {
                return ulRet;
            }
            else
            {
                /* Read in ulBytesToRead bytes from pPacketBuffer */
                memcpy(pBuf, pInt->pPacketBuffer, ulBytesToRead);
                /* Advance the current file offset and buffer offset */
                pInt->ulCurBufferOffset += ulBytesToRead;
                pInt->ulCurFileOffset += ulBytesToRead;
                return ulBytesToRead;
            }
        }
        else
        {
            /* Read in ulBytesToRead bytes from pPacketBuffer */
            memcpy(pBuf, pInt->pPacketBuffer+pInt->ulCurBufferOffset, ulBytesToRead);
            /* Advance the current file offset and buffer offset */
            pInt->ulCurBufferOffset += ulBytesToRead;
            pInt->ulCurFileOffset += ulBytesToRead;
            return ulBytesToRead;
        }
    }

    return ulRet;
}

void rm_parseri_file_seek(rm_parser_internal* pInt, UINT32 ulOffset, UINT32 ulOrigin)
{
    if (pInt && pInt->fpSeek)
    {
        /* Seek the file */
        pInt->fpSeek(pInt->pUserRead, ulOffset, ulOrigin);
        /* Update the current file offset */
        if (ulOrigin == HX_SEEK_ORIGIN_SET)
        {
            pInt->ulCurFileOffset = ulOffset;
        }
        else if (ulOrigin == HX_SEEK_ORIGIN_CUR)
        {
            pInt->ulCurFileOffset += ulOffset;
        }
        else if (ulOrigin == HX_SEEK_ORIGIN_END)
        {
            /*
             * XXXMEH - don't know file size, so not sure what to do here.
             * We could require ftell-like functionality, or we could
             * require some type of fstat-like functionality.
             */
            rm_parseri_error(pInt, HXR_NOT_SUPPORTED, "File seek with end origin attempted - not suported.");
        }
    }
}

HX_RESULT rm_parseri_copy_stream_header(rm_parser_internal* pInt, UINT32 i, rm_stream_header* pHdr)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pHdr && i < pInt->ulNumStreams && pInt->pStreamHdr)
    {
        UINT32 j      = 0;
        UINT32 ulSize = 0;
        /* Clean up any existing header */
        rm_parseri_cleanup_stream_header(pInt, pHdr);
        /* Copy the hard-coded members */
        pHdr->ulStreamNumber  = pInt->pStreamHdr[i].ulStreamNumber;
        pHdr->ulMaxBitRate    = pInt->pStreamHdr[i].ulMaxBitRate;
        pHdr->ulAvgBitRate    = pInt->pStreamHdr[i].ulAvgBitRate;
        pHdr->ulMaxPacketSize = pInt->pStreamHdr[i].ulMaxPacketSize;
        pHdr->ulAvgPacketSize = pInt->pStreamHdr[i].ulAvgPacketSize;
        pHdr->ulDuration      = pInt->pStreamHdr[i].ulDuration;
        pHdr->ulPreroll       = pInt->pStreamHdr[i].ulPreroll;
        pHdr->ulStartTime     = pInt->pStreamHdr[i].ulStartTime;
        pHdr->pMimeType       = copy_string(pInt->pUserMem, pInt->fpMalloc,
                                            pInt->pStreamHdr[i].pMimeType);
        pHdr->pStreamName     = copy_string(pInt->pUserMem, pInt->fpMalloc,
                                            pInt->pStreamHdr[i].pStreamName);
        pHdr->pOpaqueData     = copy_buffer(pInt->pUserMem, pInt->fpMalloc,
                                            pInt->pStreamHdr[i].pOpaqueData,
                                            pInt->pStreamHdr[i].ulOpaqueDataLen);
        if ((!pInt->pStreamHdr[i].pMimeType   || pHdr->pMimeType)   &&
            (!pInt->pStreamHdr[i].pStreamName || pHdr->pStreamName) &&
            (!pInt->pStreamHdr[i].pOpaqueData || pHdr->pOpaqueData))
        {
            /* Copy the properties array */
            if (pInt->pStreamHdr[i].ulNumProperties &&
                pInt->pStreamHdr[i].pProperty)
            {
                /* Allocate space for the property array */
                ulSize = pInt->pStreamHdr[i].ulNumProperties * sizeof(rm_property);
                pHdr->pProperty = (rm_property*) rm_parseri_malloc(pInt, ulSize);
                if (pHdr->pProperty)
                {
                    /* NULL out the array */
                    memset(pHdr->pProperty, 0, ulSize);
                    /* Copy each of the properties */
                    retVal = HXR_OK;
                    for (j = 0; j < pInt->pStreamHdr[i].ulNumProperties && retVal == HXR_OK; j++)
                    {
                        retVal = rm_parseri_set_rm_property(pInt, &pHdr->pProperty[j],
                                                            pInt->pStreamHdr[i].pProperty[j].pName,
                                                            pInt->pStreamHdr[i].pProperty[j].ulType,
                                                            (void*) pInt->pStreamHdr[i].pProperty[j].pValue,
                                                            pInt->pStreamHdr[i].pProperty[j].ulValueLen);
                    }
                    if (retVal == HXR_OK)
                    {
                        /* Assign the number of properties */
                        pHdr->ulNumProperties = pInt->pStreamHdr[i].ulNumProperties;
                    }
                }
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_update_seek_table(rm_parser_internal* pInt, UINT32 ulStreamNum,
                                       UINT32 ulTime, UINT32 ulOffset, UINT32 ulFlags)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->pStreamInfo && ulStreamNum < pInt->ulNumStreams)
    {
        struct rm_seek_table* pTable = &pInt->pStreamInfo[ulStreamNum].seekTable;
        if (pTable) //lint  !e774
        {
            /* Was this a keyframe packet? */
            if (ulFlags & HX_KEYFRAME_FLAG)
            {
                /* Do we have space left in the table? */
                if (pTable->pEntry && pTable->ulNumEntries < pTable->ulMaxEntries)
                {
                    /*
                     * If this is the first entry; or if this
                     * time is at least ulTimeGranularity greater
                     * than the last time added, then make an entry.
                     */
                    if (!pTable->ulNumEntries ||
                        (ulTime > pTable->ulLastTime &&
                         ulTime - pTable->ulLastTime > pTable->ulTimeGranularity))
                    {
                        /* Update the last time */
                        pTable->ulLastTime = ulTime;
                        /* Update the time range this table covers */
                        if (ulTime > pTable->ulRangeTime)
                        {
                            pTable->ulRangeTime = ulTime;
                        }
                        /* Make the new entry */
                        pTable->pEntry[pTable->ulNumEntries].ulTime   = ulTime;
                        pTable->pEntry[pTable->ulNumEntries].ulOffset = ulOffset;
                        /* Increment the number of entries */
                        pTable->ulNumEntries++;
                        /* Clear the return value */
                        retVal = HXR_OK;
                    }
                }
            }
            else
            {
                /*
                 * This is not a keyframe, so we don't have to
                 * make an entry in the table, but we do need to
                 * update the time range that it covers.
                 */
                if (ulTime > pTable->ulRangeTime)
                {
                    pTable->ulRangeTime = ulTime;
                }
                /* Clear the return value */
                retVal = HXR_OK;
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_update_time_range(rm_parser_internal* pInt, UINT32 ulStreamNum,
                                       UINT32 ulTime)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->pStreamInfo && ulStreamNum < pInt->ulNumStreams)
    {
        /* Get the seek table for this stream */
        struct rm_seek_table* pTable = &pInt->pStreamInfo[ulStreamNum].seekTable;
        if (pTable) //lint  !e774
        {
            /* Update the time range this table covers */
            if (ulTime > pTable->ulRangeTime)
            {
                pTable->ulRangeTime = ulTime;
            }
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_search_all_seek_tables(rm_parser_internal* pInt, UINT32 ulSeekTime,
                                            UINT32* pulFoundTime, UINT32* pulFoundOffset)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pulFoundTime && pulFoundOffset &&
        pInt->ulNumStreams && pInt->pStreamInfo)
    {
        /* Initialize local variables */
        UINT32    ulMaxTime   = 0;
        UINT32    ulMaxOffset = 0;
        UINT32    i           = 0;
        HX_RESULT status      = HXR_FAIL;
        /* Search the streams */
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            /* Get the stream info struct */
            struct rm_stream_info* pInfo = &pInt->pStreamInfo[i];
            if (pInfo) //lint  !e774
            {
                status = rm_parseri_search_seek_table(&pInfo->seekTable,
                                                      ulSeekTime,
                                                      &pInfo->keyFramePacket.ulTimestamp,
                                                      &pInfo->keyFramePacket.ulFileOffset);
                if (status == HXR_OK)
                {
                    pInfo->keyFramePacket.bValid = TRUE;
                    if (pInfo->keyFramePacket.ulTimestamp > ulMaxTime)
                    {
                        ulMaxTime = pInfo->keyFramePacket.ulTimestamp;
                    }
                    if (pInfo->keyFramePacket.ulFileOffset > ulMaxOffset)
                    {
                        ulMaxOffset = pInfo->keyFramePacket.ulFileOffset;
                    }
                }
                /*
                 * If at least one stream succeeds, the overall search
                 * succeeds. Note that rm_parseri_search_seek_table() can
                 * return either HXR_AT_END or HXR_OK, both of which
                 * return HX_SUCCEEDED() == TRUE. That's why we have to
                 * specifically check for status == HXR_OK, as opposed
                 * to just HX_SUCCEEDED(status).
                 */
                if (HX_FAILED(retVal) ||
                    (HX_SUCCEEDED(retVal) && status == HXR_OK))
                {
                    retVal = status;
                }
            }
        }
        if (HX_SUCCEEDED(retVal))
        {
            *pulFoundTime   = ulMaxTime;
            *pulFoundOffset = ulMaxOffset;
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_search_seek_table(struct rm_seek_table* pTable, UINT32 ulSeekTime,
                                       UINT32* pulFoundTime, UINT32* pulFoundOffset)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pTable && pulFoundTime && pulFoundOffset && pTable->ulNumEntries)
    {
        /* Estimate index to seek from */
        UINT32 ulTableIdx = ulSeekTime / pTable->ulTimeGranularity;
        /*
         * If we've over-estimated, set the index at the
         * last entry in the table.
         */
        if (ulTableIdx >= pTable->ulNumEntries)
        {
            ulTableIdx = pTable->ulNumEntries - 1;
        }
        /* Scan forward in the table */
        while (((ulTableIdx + 1) < (pTable->ulNumEntries - 1)) &&
	       (pTable->pEntry[ulTableIdx + 1].ulTime < ulSeekTime))
        {
	    ulTableIdx++;
        }
        /* Scan backwards in the table */
        while ((ulTableIdx > 0) &&
	       (pTable->pEntry[ulTableIdx].ulTime > ulSeekTime))
        {
	    ulTableIdx--;
        }
        /* Report results */
        pTable->ulCurEntry = ulTableIdx;
        *pulFoundTime   = pTable->pEntry[ulTableIdx].ulTime;
        *pulFoundOffset = pTable->pEntry[ulTableIdx].ulOffset;
        /* Set the return value */
        retVal = (ulSeekTime > pTable->ulRangeTime ? HXR_AT_END : HXR_OK);
    }

    return retVal;
}
HX_RESULT rm_parseri_search_seek_table_forward(rm_parser_internal* pInt,struct rm_seek_table* pTable, UINT32 ulSeekTime,
                                       UINT32* pulFoundTime, UINT32* pulFoundOffset)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pTable && pulFoundTime && pulFoundOffset && pTable->ulNumEntries)
    {
        /* Estimate index to seek from */
        UINT32 ulTableIdx = ulSeekTime / pTable->ulTimeGranularity;
        /*
         * If we've over-estimated, set the index at the
         * last entry in the table.
         */
        rm_parseri_error(pInt, pTable->ulNumEntries, "ulNumEntries");
        rm_parseri_error(pInt, pTable->ulRangeTime, "ulRangeTime");
        rm_parseri_error(pInt, ulTableIdx, "ulTableIdx");
        if (ulTableIdx >= pTable->ulNumEntries)
        {
            ulTableIdx = pTable->ulNumEntries - 1;
        }
        rm_parseri_error(pInt, ulTableIdx, "ulTableIdx");
        rm_parseri_error(pInt, pTable->pEntry[ulTableIdx].ulTime, "ulTime");
        /* Scan forward in the table */
        while (((ulTableIdx + 1) < (pTable->ulNumEntries - 1)) &&
	       (pTable->pEntry[ulTableIdx + 1].ulTime < ulSeekTime))
        {
	    ulTableIdx++;
        }
        /* Scan backwards in the table */
        while ((ulTableIdx > 0) &&
	       (pTable->pEntry[ulTableIdx].ulTime > ulSeekTime))
        {
	    ulTableIdx--;
        }
        /* Report results */
        rm_parseri_error(pInt, ulTableIdx, "ulTableIdx");
        rm_parseri_error(pInt, pTable->pEntry[ulTableIdx].ulTime, "ulTime");
        *pulFoundTime   = pTable->pEntry[ulTableIdx+1].ulTime;
        *pulFoundOffset = pTable->pEntry[ulTableIdx+1].ulOffset;
        /* Set the return value */
        retVal = (ulSeekTime > pTable->ulRangeTime ? HXR_AT_END : HXR_OK);
    }

    return retVal;
}

HX_RESULT rm_parseri_search_index_chunk(rm_parser_internal* pInt, UINT32 ulSeekTime)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->propHdr.index_offset && pInt->pStreamInfo &&
        pInt->ulNumStreams)
    {
        /* Initialize local variables */
        struct rm_index_hdr hdr;
        struct rm_index_rec rec;
        UINT32 ulChunkID     = 0;
        UINT32 ulStreamNum   = 0;
        UINT32 i             = 0;
        UINT32 ulIndexOffset = pInt->propHdr.index_offset;
        /* Mark all keyframe packets as invalid */
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            pInt->pStreamInfo[i].keyFramePacket.bValid = FALSE;
        }
        /* Clear the return value */
        retVal = HXR_OK;
        /* Loop through all the index chunks */
        while (HX_SUCCEEDED(retVal) &&
               ulIndexOffset     &&
               !rm_parseri_is_all_keyframes_found(pInt))
        {
            /* Seek the file to the next index chunk */
            rm_parseri_error(pInt, ulIndexOffset, "ulIndexOffset");
            rm_parseri_file_seek(pInt, ulIndexOffset, HX_SEEK_ORIGIN_SET);
            /* Read the index header */
            retVal = rm_parseri_read_next_header(pInt, &ulChunkID);
            rm_parseri_error(pInt, retVal, "retVal");
            rm_parseri_error(pInt, ulChunkID, "ulChunkID");
            if (retVal == HXR_OK)
            {
                /* Assume the worst */
                retVal = HXR_FAIL;
                /* Make sure this is an INDX header */
                if (ulChunkID == RM_INDEX_OBJECT)
                {
                    /* Parse the index header */
                    retVal = rm_parseri_unpack_index_hdr(pInt, &hdr);
                    if (retVal == HXR_OK)
                    {
                        /*
                         * Translate the stream number. If the
                         * stream number doesn't translate, then
                         * we will not consider it an error - we'll
                         * just go on to the next index chunk.
                         */
                        ulStreamNum = rm_parseri_translate_stream_number(pInt, hdr.stream_num);
                        if (ulStreamNum != RM_NO_STREAM_SET)
                        {
                            /* Get the rm_keyframe_packet struct */
                            struct rm_keyframe_packet* pKey =
                                &pInt->pStreamInfo[ulStreamNum].keyFramePacket;
                            /* Read all the index records in this chunk */
                            rm_parseri_error(pInt, ulStreamNum, "ulStreamNum");
                            rm_parseri_error(pInt, hdr.num_recs, "hdr.num_recs");
                            for (i = 0; i < hdr.num_recs && retVal == HXR_OK; i++)
                            {
                                retVal = rm_parseri_read_next_index_rec(pInt, &rec);
                                if (retVal == HXR_OK)
                                {
                                    /*
                                     * Set this record into the on-the-fly
                                     * seek table.
                                     */
                                    rm_parseri_update_seek_table(pInt,
                                                                 ulStreamNum,
                                                                 rec.timestamp,
                                                                 rec.offset,
                                                                 HX_KEYFRAME_FLAG);
                                    /*
                                     * Is the timestamp of the record
                                     * greater than or equal to the seek time?
                                     */
                                    rm_parseri_error(pInt, rec.timestamp, "rec.timestamp");
                                    rm_parseri_error(pInt, rec.offset, "rec.offset");
                                    if (rec.timestamp >= ulSeekTime)
                                    {
                                        /* Do we have a valid keyframe for this stream? */
                                        //if (!pKey->bValid)
                                        {
                                            pKey->bValid       = TRUE;
                                            pKey->ulFileOffset = rec.offset;
                                            pKey->ulTimestamp  = rec.timestamp;
                                        }
                                        /*
                                         * Now we can stop looking at index records
                                         * for this stream
                                         */
                                        break;
                                    }
                                    else
                                    {
                                        /* Save the keyframe info */
                                        pKey->bValid       = TRUE;
                                        pKey->ulFileOffset = rec.offset;
                                        pKey->ulTimestamp  = rec.timestamp;
                                    }
                                }
                            }
                        }
                        /* 
                         * Set the next index header. If there are
                         * no more index chunks, next_index_hdr will
                         * be zero.
                         */
                        ulIndexOffset = hdr.next_index_hdr;
                    }
                }
            }
        }
        /* Find the first keyframe */
        if (HX_SUCCEEDED(retVal))
        {
            retVal = rm_parseri_find_first_keyframe(pInt);
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_search_stream_index_chunk(rm_parser_internal* pInt, UINT32 ulSeekTime, UINT32 ulStream)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->propHdr.index_offset && pInt->pStreamInfo &&
        pInt->ulNumStreams)
    {
        /* Initialize local variables */
        struct rm_index_hdr hdr;
        struct rm_index_rec rec;
        UINT32 ulChunkID     = 0;
        UINT32 ulStreamNum   = 0;
        UINT32 i             = 0;
        UINT32 ulIndexOffset = pInt->propHdr.index_offset;
        /* Mark all keyframe packets as invalid */
        //for (i = 0; i < pInt->ulNumStreams; i++)
        {
            pInt->pStreamInfo[ulStream].keyFramePacket.bValid = FALSE;
        }
        /* Clear the return value */
        retVal = HXR_OK;
        /* Loop through all the index chunks */
        while (HX_SUCCEEDED(retVal) &&
               ulIndexOffset     &&
               !pInt->pStreamInfo[ulStream].keyFramePacket.bValid)
        {
            /* Seek the file to the next index chunk */
            rm_parseri_error(pInt, ulIndexOffset, "ulIndexOffset");
            rm_parseri_file_seek(pInt, ulIndexOffset, HX_SEEK_ORIGIN_SET);
            /* Read the index header */
            retVal = rm_parseri_read_next_header(pInt, &ulChunkID);
            rm_parseri_error(pInt, retVal, "retVal");
            rm_parseri_error(pInt, ulChunkID, "ulChunkID");
            if (retVal == HXR_OK)
            {
                /* Assume the worst */
                retVal = HXR_FAIL;
                /* Make sure this is an INDX header */
                if (ulChunkID == RM_INDEX_OBJECT)
                {
                    /* Parse the index header */
                    retVal = rm_parseri_unpack_index_hdr(pInt, &hdr);
                    if (retVal == HXR_OK)
                    {
                        /*
                         * Translate the stream number. If the
                         * stream number doesn't translate, then
                         * we will not consider it an error - we'll
                         * just go on to the next index chunk.
                         */
                        ulStreamNum = rm_parseri_translate_stream_number(pInt, hdr.stream_num);
                        if (ulStreamNum == ulStream)
                        {
                            /* Get the rm_keyframe_packet struct */
                            struct rm_keyframe_packet* pKey =
                                &pInt->pStreamInfo[ulStreamNum].keyFramePacket;
                            /* Read all the index records in this chunk */
                            rm_parseri_error(pInt, ulStreamNum, "ulStreamNum");
                            rm_parseri_error(pInt, hdr.num_recs, "hdr.num_recs");
                            for (i = 0; i < hdr.num_recs && retVal == HXR_OK; i++)
                            {
                                retVal = rm_parseri_read_next_index_rec(pInt, &rec);
                                if (retVal == HXR_OK)
                                {
                                    /*
                                     * Set this record into the on-the-fly
                                     * seek table.
                                     */
                                    rm_parseri_update_seek_table(pInt,
                                                                 ulStreamNum,
                                                                 rec.timestamp,
                                                                 rec.offset,
                                                                 HX_KEYFRAME_FLAG);
                                    /*
                                     * Is the timestamp of the record
                                     * greater than or equal to the seek time?
                                     */
                                    rm_parseri_error(pInt, rec.timestamp, "rec.timestamp");
                                    rm_parseri_error(pInt, rec.offset, "rec.offset");
                                    if (rec.timestamp >= ulSeekTime)
                                    {
                                        /* Do we have a valid keyframe for this stream? */
                                        //if(pInt->pStreamInfo[ulStream].bIsRealVideo) //audio behind video
                                        {
                                            pKey->bValid       = TRUE;
                                            pKey->ulFileOffset = rec.offset;
                                            pKey->ulTimestamp  = rec.timestamp;
                                        }
                                        /*
                                         * Now we can stop looking at index records
                                         * for this stream
                                         */
                                        break;
                                    }
                                    else
                                    {
                                        /* Save the keyframe info */
                                        pKey->bValid       = TRUE;
                                        pKey->ulFileOffset = rec.offset;
                                        pKey->ulTimestamp  = rec.timestamp;
                                    }
                                }
                            }
                        }
                        /* 
                         * Set the next index header. If there are
                         * no more index chunks, next_index_hdr will
                         * be zero.
                         */
                        ulIndexOffset = hdr.next_index_hdr;
                    }
                }
            }
        }
        /* Find the first keyframe */
        
    }

    return retVal;
}

HX_RESULT rm_parseri_seek_audio(rm_parser_internal* pInt, UINT32 ulSeekTime, UINT32 ulStreamNum)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        HX_RESULT status       = HXR_OK;
        UINT32    ulDataOffset = 0;
        struct rm_stream_info* pInfo;
        pInfo = &pInt->pStreamInfo[ulStreamNum];
        status = rm_parseri_search_seek_table_forward(pInt, &pInfo->seekTable, //audio in front of video
                                          ulSeekTime,
                                          &pInfo->keyFramePacket.ulTimestamp,
                                          &pInfo->keyFramePacket.ulFileOffset);
        rm_parseri_error(pInt, status, "status");
        rm_parseri_error(pInt, ulSeekTime, "ulAudSeekTime");
        rm_parseri_error(pInt, pInfo->keyFramePacket.ulTimestamp, "keyFramePacket.ulTimestamp");
        rm_parseri_error(pInt, pInfo->keyFramePacket.ulFileOffset, "keyFramePacket.ulFileOffset");
        if(status == HXR_OK)
        {
            pInfo->keyFramePacket.bValid = TRUE;
            retVal = rm_parseri_find_first_keyframe(pInt);
        }
        else if(pInt->audioIndexInfo.bHaveIndex&&ulSeekTime)//(pInt->propHdr.index_offset && ulSeekTime)
        {
            #if 0
            retVal = rm_parseri_search_stream_index_chunk(pInt, ulSeekTime, ulStreamNum);//audio in front of video
            #else
            retVal = rm_parseri_search_index_in_buffer(pInt, ulSeekTime, ulStreamNum);
            #endif
            /* Find the first keyframe */
            if (HX_SUCCEEDED(retVal))
            {
                retVal = rm_parseri_find_first_keyframe(pInt);
            }
        }
        else if(status == HXR_AT_END)
        {
            pInfo->keyFramePacket.bValid = TRUE;
            retVal = rm_parseri_find_first_packet_after_seek_time(pInt,  //audio behind video
                                                      ulSeekTime,
                                                      pInfo->keyFramePacket.ulFileOffset);
        }
        else
        {
            //seekTable is empty
            ulDataOffset = pInt->propHdr.data_offset +
                           RM_PARSER_DATA_CHUNK_HEADER_SIZE;
            /* Seek from the beginning of the data chunk */
            retVal = rm_parseri_find_first_packet_after_seek_time(pInt,
                                                                  ulSeekTime,
                                                                  ulDataOffset);
        }
    }
    return retVal;
}

HX_RESULT rm_parseri_seek(rm_parser_internal* pInt, UINT32 ulSeekTime)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->ulNumStreams && pInt->pStreamInfo)
    {
        /* Initialize local variables */
        UINT32    i            = 0;
        HX_RESULT status       = HXR_OK;
        //UINT32    ulFoundTime  = 0;
        UINT32    ulDataOffset = 0;
        UINT32    ulAudSeekTime = 0;
        struct rm_stream_info* pInfo;
        UINT32    ulAudStream = pInt->ulAudioStreamNum;
        UINT32    ulVidStream = pInt->ulVideoStreamNum;
        /* Clear out stream state */
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            pInt->pStreamInfo[i].bStreamDone           = FALSE;
            pInt->pStreamInfo[i].ulLastTimeStamp       = 0xFFFFFFFF;
            pInt->pStreamInfo[i].keyFramePacket.bValid = FALSE;
        }
#if 0
        /* First we try to search the on-the-fly seek table */
        status = rm_parseri_search_all_seek_tables(pInt, ulSeekTime,
                                                   &ulFoundTime, &ulDataOffset);
        rm_parseri_error(pInt, status, "search_all_seek_tables status");

        if (status == HXR_OK)
        {
            retVal = rm_parseri_find_first_packet_after_seek_time(pInt,
                                                                  ulSeekTime,
                                                                  ulDataOffset);
        }
        else if (pInt->propHdr.index_offset && ulSeekTime)
        {
            /*
             * Try to find the offset via the index chunks
             * at the end of the file
             */
            retVal = rm_parseri_search_index_chunk(pInt, ulSeekTime);
        }
        else if (status == HXR_AT_END)
        {
            /*
             * The on-the-fly seek table returned HXR_AT_END,
             * which means the seek time was past it's range,
             * so it returned the last entry it had in the table.
             * If we can't use the index table at the end of the
             * file, then we'll use this - it's better than starting
             * at the beginning of the data chunk.
             */
            for (i = 0; i < pInt->ulNumStreams; i++)
            {
                if(pInt->pStreamInfo[i].keyFramePacket.ulFileOffset > ulDataOffset)
                {
                    ulDataOffset = pInt->pStreamInfo[i].keyFramePacket.ulFileOffset;
                }
            }
            retVal = rm_parseri_find_first_packet_after_seek_time(pInt,
                                                                  ulSeekTime,
                                                                  ulDataOffset);
        }
        else
        {
            /*
             * Compute the offset of the first packet
             * after the data chunk.
             */
            ulDataOffset = pInt->propHdr.data_offset +
                           RM_PARSER_DATA_CHUNK_HEADER_SIZE;
            /* Seek from the beginning of the data chunk */
            retVal = rm_parseri_find_first_packet_after_seek_time(pInt,
                                                                  ulSeekTime,
                                                                  ulDataOffset);
        }
#else
        pInfo = &pInt->pStreamInfo[ulVidStream];
        rm_parseri_error(pInt, pInfo->bIsRealVideo, "bIsRealVideo");
        if(pInfo->bIsRealVideo)
        {
            status = rm_parseri_search_seek_table(&pInfo->seekTable,
                                                  ulSeekTime,
                                                  &pInfo->keyFramePacket.ulTimestamp,
                                                  &pInfo->keyFramePacket.ulFileOffset);
            rm_parseri_error(pInt, status, "status");
            rm_parseri_error(pInt, ulSeekTime, "ulSeekTime");
            rm_parseri_error(pInt, pInfo->keyFramePacket.ulTimestamp, "keyFramePacket.ulTimestamp");
            if(status == HXR_OK)
            {
                pInfo->keyFramePacket.bValid = TRUE;
                ulAudSeekTime = pInfo->keyFramePacket.ulTimestamp;
                retVal = rm_parseri_seek_audio(pInt, ulAudSeekTime, ulAudStream);
            }
            else if(pInt->videoIndexInfo.bHaveIndex&&ulSeekTime)//(pInt->propHdr.index_offset && ulSeekTime)
            {
                #if 0
                status = rm_parseri_search_stream_index_chunk(pInt, ulSeekTime, ulVidStream);
                #else
                status = rm_parseri_search_index_in_buffer(pInt, ulSeekTime, ulVidStream);
                #endif
                if(status == HXR_OK)
                {
                    ulAudSeekTime = pInfo->keyFramePacket.ulTimestamp;
                    retVal = rm_parseri_seek_audio(pInt, ulAudSeekTime, ulAudStream);
                }

            }
            else if(status == HXR_AT_END)
            {
                //pInfo->keyFramePacket.bValid = TRUE;
                retVal = rm_parseri_find_first_packet_after_seek_time(pInt,
                                                              ulSeekTime,
                                                              pInfo->keyFramePacket.ulFileOffset);

                pInfo->keyFramePacket.bValid = TRUE;
                ulAudSeekTime = pInfo->keyFramePacket.ulTimestamp;
                retVal = rm_parseri_seek_audio(pInt, ulAudSeekTime, ulAudStream);        
            }
            else
            {
                /*
                 * Compute the offset of the first packet
                 * after the data chunk.
                 */
                //seekTable is empty
                ulDataOffset = pInt->propHdr.data_offset +
                               RM_PARSER_DATA_CHUNK_HEADER_SIZE;
                /* Seek from the beginning of the data chunk */
                retVal = rm_parseri_find_first_packet_after_seek_time(pInt,
                                                                      ulSeekTime,
                                                                      ulDataOffset);
            }
        }
        else
        {
            rm_parseri_error(pInt, ulVidStream, "no video stream");
            return retVal;
        }
#endif
    }

    return retVal;
}

HXBOOL rm_parseri_is_all_keyframes_found(rm_parser_internal* pInt)
{
    HXBOOL bRet = FALSE;

    if (pInt && pInt->ulNumStreams && pInt->pStreamInfo)
    {
        /* Init local variables */
        UINT32 i = 0;
        /* Assume we have found all of them */
        bRet = TRUE;
        /* Look through all streams */
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            if (!pInt->pStreamInfo[i].keyFramePacket.bValid)
            {
                bRet = FALSE;
                break;
            }
        }
    }

    return bRet;
}

HXBOOL rm_parseri_is_a_keyframe_found(rm_parser_internal* pInt)
{
    HXBOOL bRet = FALSE;

    if (pInt && pInt->ulNumStreams && pInt->pStreamInfo)
    {
        /* Init local variables */
        UINT32 i = 0;
        /* Look through all streams */
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            if (pInt->pStreamInfo[i].keyFramePacket.bValid)
            {
                bRet = TRUE;
                break;
            }
        }
    }

    return bRet;
}

HX_RESULT rm_parseri_find_first_packet_after_seek_time(rm_parser_internal* pInt,
                                                       UINT32 ulSeekTime,
                                                       UINT32 ulInitialOffset)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt)
    {
        UINT32 ulStreamNum = 0;
        UINT32 ulOffset    = 0;
        struct rm_pkt_hdr hdr;
        /* Clear the return value */
        retVal = HXR_OK;
        /* Seek to the initial offset */
        rm_parseri_file_seek(pInt, ulInitialOffset, HX_SEEK_ORIGIN_SET);
        /*
         * Read packet headers until we find one
         * which is greater than or equal to the seek time.
         */
         rm_parseri_error(pInt, ulInitialOffset, "ulInitialOffset");
        while (HX_SUCCEEDED(retVal))
        {
            /* Read the next packet header */
            retVal = rm_parseri_read_next_packet_header(pInt, &hdr);
            rm_parseri_error(pInt, retVal, "retVal");
            if (HX_SUCCEEDED(retVal))
            {
                /* Compute the offset of this packet */
                ulOffset = pInt->ulCurFileOffset - RM_PARSER_PACKET_HEADER_SIZE;
                /*
                 * Is the timestamp of this packet greater
                 * than or equal to the seek time?
                 */
                rm_parseri_error(pInt, ulOffset, "ulOffset");
                rm_parseri_error(pInt, hdr.timestamp, "timestamp");
                if (hdr.timestamp >= ulSeekTime)
                {
                    /* We're finished. */
                    retVal = rm_parseri_find_first_keyframe(pInt);
                    break;
                }
                else
                {
                    /* Assume the worst */
                    retVal = HXR_FAIL;
                    /* Get the stream num */
                    ulStreamNum = rm_parseri_translate_stream_number(pInt, hdr.stream_num);
                    if (ulStreamNum != RM_NO_STREAM_SET)
                    {
                        /* Get the rm_keyframe_packet struct for this stream */
                        struct rm_keyframe_packet* pKey = &pInt->pStreamInfo[ulStreamNum].keyFramePacket;
                        if (pKey) //lint  !e774
                        {
                            rm_parseri_error(pInt, ulStreamNum, "ulStreamNum");
                            rm_parseri_error(pInt, hdr.flags, "hdr.flags");
                            rm_parseri_error(pInt, pKey->bValid, "pKey->bValid");
                            rm_parseri_error(pInt, pKey->ulTimestamp, "pKey->ulTimestamp");
                            if (hdr.flags & HX_KEYFRAME_FLAG)
                            {
                                if (!pKey->bValid || hdr.timestamp > pKey->ulTimestamp)
                                {
                                    /* Save the keyframe info */
                                    pKey->ulTimestamp  = hdr.timestamp;
                                    pKey->ulFileOffset = ulOffset;
                                    pKey->bValid       = TRUE;
                                    /* Update the on-the-fly index table */
                                    rm_parseri_update_seek_table(pInt,
                                                                 ulStreamNum,
                                                                 hdr.timestamp,
                                                                 ulOffset,
                                                                 hdr.flags);
                                }
                            }
                            else
                            {
                                rm_parseri_update_time_range(pInt,
                                                             ulStreamNum,
                                                             hdr.timestamp);
                            }
                            /* Clear the return value */
                            retVal = HXR_OK;
                        }
                    }
                }
                /*
                 * If we've succeeded and we're not done,
                 * then seek to the next packet header.
                 */
                if (HX_SUCCEEDED(retVal))
                {
                    rm_parseri_file_seek(pInt,
                                         hdr.length - RM_PARSER_PACKET_HEADER_SIZE,
                                         HX_SEEK_ORIGIN_CUR);
                }
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_find_first_keyframe(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->pStreamInfo && pInt->ulNumStreams)
    {
        UINT32 ulLowOffset = 0;
        UINT32 ulLowStream = 0;
        UINT32 ulNumValid  = 0;
        UINT32 i           = 0;
        /*
         * Mark the number of keyframes needed as
         * the number of streams.
         */
        pInt->ulKeyframesNeeded = pInt->ulNumStreams;
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            pInt->pStreamInfo[i].bNeedKeyframe = TRUE;
        }
        /* Find the lowest offset of the valid keyframes */
        for (i = 0; i < pInt->ulNumStreams; i++)
        {
            if (pInt->pStreamInfo[i].bNeedKeyframe &&
                pInt->pStreamInfo[i].keyFramePacket.bValid)
            {
                /* Increment the number of valid keyframes */
                ulNumValid++;
                /* Find the lowest keyframe time and stream */
                if (!ulLowOffset ||
                    pInt->pStreamInfo[i].keyFramePacket.ulFileOffset < ulLowOffset)
                {
                    ulLowOffset = pInt->pStreamInfo[i].keyFramePacket.ulFileOffset;
                    ulLowStream = i;
                }
            }
        }
        /* Did we find any valid keyframes? */
        if (ulNumValid)
        {
            /* Seek to the lowest offset */
            rm_parseri_file_seek(pInt, ulLowOffset, HX_SEEK_ORIGIN_SET);
            /* Clear the return value */
            retVal = HXR_OK;
        }
        else
        {
            /* Clear the number of keyframes needed */
            pInt->ulKeyframesNeeded = 0;
        }
    }

    return retVal;
}

void* rm_parseri_malloc(rm_parser_internal* pInt, UINT32 ulSize)
{
    void* pRet = HXNULL;

    if (pInt && pInt->fpMalloc)
    {
        pRet = pInt->fpMalloc(pInt->pUserMem, ulSize);
    }

    return pRet;
}

void rm_parseri_free(rm_parser_internal* pInt, void* pMem)
{
    if (pInt && pInt->fpFree)
    {
        pInt->fpFree(pInt->pUserMem, pMem);
    }
}

void rm_parseri_error(rm_parser_internal* pInt, HX_RESULT err, const char* pszMsg)
{
    if (pInt && pInt->fpError)
    {
        pInt->fpError(pInt->pUserError, err, pszMsg);
    }
}

HX_RESULT rm_parseri_unpack_index_rec_in_buffer(BYTE* pIndexBuffer,
                                      struct rm_index_rec* rec)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pIndexBuffer && rec)
    {
        /* Assign temporary variables */
        //BYTE*  pBuf  = (BYTE*)pIndexBuffer;
        UINT32 ulLen = RM_PARSER_INDEX_RECORD_SIZE;

            /* Unpack the rm_index_rec */
            rec->version   = rm_unpack16(&pIndexBuffer, &ulLen);
            rec->timestamp = rm_unpack32(&pIndexBuffer, &ulLen);
            rec->offset    = rm_unpack32(&pIndexBuffer, &ulLen);
            rec->num_pkts  = rm_unpack32(&pIndexBuffer, &ulLen);
            /* Clear the return value */
            retVal = HXR_OK;
    }

    return retVal;
}

HX_RESULT rm_parseri_index_load(rm_parser_internal* pInt)
{
    HX_RESULT retVal = HXR_FAIL;

    if(pInt)
    {
        /* Initialize local variables */
        struct rm_index_hdr hdr;
        struct rm_index_rec rec;
        UINT32 ulChunkID     = 0;
        UINT32 ulStreamNum   = 0;
        UINT32 ulIndexOffset = pInt->propHdr.index_offset;
        UINT32    ulAudStream = pInt->ulAudioStreamNum;
        UINT32    ulVidStream = pInt->ulVideoStreamNum;
        UINT32    ulBytesToRead = 0;
        UINT32    ulRet = 0;
        BYTE*  pIndexBuf = HXNULL;
        UINT32    ulCurFileOffset = pInt->ulCurFileOffset;

        retVal = HXR_OK;
        
        while(HX_SUCCEEDED(retVal) && ulIndexOffset)
        {
            rm_parseri_file_seek(pInt, ulIndexOffset, HX_SEEK_ORIGIN_SET);
                /* Read the index header */
            retVal = rm_parseri_read_next_header(pInt, &ulChunkID);
            if (retVal == HXR_OK)
            {
                retVal = HXR_FAIL;
                if(ulChunkID == RM_INDEX_OBJECT)
                {
                    retVal = rm_parseri_unpack_index_hdr(pInt, &hdr);
                    if (retVal == HXR_OK)
                    {
                        ulStreamNum = rm_parseri_translate_stream_number(pInt, hdr.stream_num);
                        if(ulStreamNum == ulVidStream)
                        {
                            if(TRUE == pInt->videoIndexInfo.bHaveIndex || 0 == hdr.num_recs)
                            {
                                ulIndexOffset = hdr.next_index_hdr;
                                continue;
                            }
                            pInt->videoIndexInfo.ulStreamNum = ulStreamNum;
                            pInt->videoIndexInfo.ulNumIndexesAll = hdr.num_recs;
                            pInt->videoIndexInfo.ulIndexChunkOffset = pInt->ulCurFileOffset;
                            rm_parseri_error(pInt, ulStreamNum, "ulStreamNum");
                            rm_parseri_error(pInt, hdr.num_recs, "num_recs");
                            if(hdr.num_recs > RM_PARSER_INDEX_TABLE_LEN)
                            {           
                                ulBytesToRead = RM_PARSER_INDEX_TABLE_LEN*RM_PARSER_INDEX_RECORD_SIZE;
                                pInt->videoIndexInfo.pIndexBuffer = rm_parseri_malloc(pInt, ulBytesToRead);
                                if(HXNULL == pInt->videoIndexInfo.pIndexBuffer)
                                {
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                ulRet = pInt->fpRead(pInt->pUserRead, pInt->videoIndexInfo.pIndexBuffer, ulBytesToRead);
                                if(ulRet != ulBytesToRead)
                                {
                                    rm_parseri_free(pInt, pInt->videoIndexInfo.pIndexBuffer);
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                pIndexBuf = pInt->videoIndexInfo.pIndexBuffer;
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->videoIndexInfo.ulNumIndexes = RM_PARSER_INDEX_TABLE_LEN;
                                pInt->videoIndexInfo.ulFirstIndexTime = rec.timestamp;
                                pInt->videoIndexInfo.ulFIrstFileOffset = rec.offset;
                                pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->videoIndexInfo.ulLastIndexTime = rec.timestamp;
                                pInt->videoIndexInfo.ulLastFileOffset = rec.offset;
                                pInt->videoIndexInfo.ulFirstIndex = 0;
                                pInt->videoIndexInfo.ulLastIndex = RM_PARSER_INDEX_TABLE_LEN -1;
                                pInt->videoIndexInfo.ulCurIndex = 0;
                                pInt->videoIndexInfo.ulNextIndexOffset = pInt->ulCurFileOffset + ulBytesToRead;
                            }
                            else
                            {
                                ulBytesToRead = hdr.num_recs*RM_PARSER_INDEX_RECORD_SIZE;
                                pInt->videoIndexInfo.pIndexBuffer = rm_parseri_malloc(pInt, ulBytesToRead);
                                //rm_parseri_error(pInt, pInt->videoIndexInfo.pIndexBuffer, "pInt->videoIndexInfo.pIndexBuffer");
                                if(HXNULL == pInt->videoIndexInfo.pIndexBuffer)
                                {
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                ulRet = pInt->fpRead(pInt->pUserRead, pInt->videoIndexInfo.pIndexBuffer, ulBytesToRead);
                                if(ulRet != ulBytesToRead)
                                {
                                    rm_parseri_free(pInt, pInt->videoIndexInfo.pIndexBuffer);
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                //rm_parseri_error(pInt, hdr.num_recs, "hdr.num_recs");
                                pIndexBuf = pInt->videoIndexInfo.pIndexBuffer;
                                //rm_parseri_error(pInt, sizeof(rm_index_record), "sizeof(rm_index_record)");
                                //rm_parseri_error(pInt, &pIndex->timestamp, "&pIndex->timestamp");
                                //rm_parseri_error(pInt, pIndex, "pIndex");
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->videoIndexInfo.ulNumIndexes = hdr.num_recs;
                                pInt->videoIndexInfo.ulFirstIndexTime = rec.timestamp;
                                pInt->videoIndexInfo.ulFIrstFileOffset = rec.offset;
                                //rm_parseri_error(pInt, &pIndex[hdr.num_recs -1], "&pIndex[hdr.num_recs -1]");
                                pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                //rm_parseri_error(pInt, pIndex, "pIndex");
                                pInt->videoIndexInfo.ulLastIndexTime = rec.timestamp;
                                pInt->videoIndexInfo.ulLastFileOffset = rec.offset;
                                pInt->videoIndexInfo.ulFirstIndex = 0;
                                pInt->videoIndexInfo.ulLastIndex = hdr.num_recs -1;
                                pInt->videoIndexInfo.ulCurIndex = 0;
                                pInt->videoIndexInfo.ulNextIndexOffset = 0;
                            }
                            pInt->videoIndexInfo.bHaveIndex = TRUE;
                            rm_parseri_error(pInt, pInt->videoIndexInfo.ulFirstIndexTime, "ulFirstIndexTime");
                            rm_parseri_error(pInt, pInt->videoIndexInfo.ulFIrstFileOffset, "ulFIrstFileOffset");
                            rm_parseri_error(pInt, pInt->videoIndexInfo.ulLastIndexTime, "ulLastIndexTime");
                            rm_parseri_error(pInt, pInt->videoIndexInfo.ulLastFileOffset, "ulLastFileOffset");
                        }
                        else if(ulStreamNum == ulAudStream)
                        {
                            if(TRUE == pInt->audioIndexInfo.bHaveIndex || 0 == hdr.num_recs)
                            {
                                ulIndexOffset = hdr.next_index_hdr;
                                continue;
                            }
                            pInt->audioIndexInfo.ulStreamNum = ulStreamNum;
                            pInt->audioIndexInfo.ulNumIndexesAll = hdr.num_recs;
                            pInt->audioIndexInfo.ulIndexChunkOffset = pInt->ulCurFileOffset;
                            rm_parseri_error(pInt, ulStreamNum, "ulStreamNum");
                            rm_parseri_error(pInt, hdr.num_recs, "num_recs");
                            if(hdr.num_recs > RM_PARSER_INDEX_TABLE_LEN)
                            {           
                                ulBytesToRead = RM_PARSER_INDEX_TABLE_LEN*RM_PARSER_INDEX_RECORD_SIZE;
                                pInt->audioIndexInfo.pIndexBuffer = rm_parseri_malloc(pInt, ulBytesToRead);
                                if(HXNULL == pInt->audioIndexInfo.pIndexBuffer)
                                {
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                ulRet = pInt->fpRead(pInt->pUserRead, pInt->audioIndexInfo.pIndexBuffer, ulBytesToRead);
                                if(ulRet != ulBytesToRead)
                                {
                                    rm_parseri_free(pInt, pInt->audioIndexInfo.pIndexBuffer);
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                pIndexBuf = pInt->audioIndexInfo.pIndexBuffer;
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->audioIndexInfo.ulNumIndexes = RM_PARSER_INDEX_TABLE_LEN;
                                pInt->audioIndexInfo.ulFirstIndexTime = rec.timestamp;
                                pInt->audioIndexInfo.ulFIrstFileOffset = rec.offset;
                                pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->audioIndexInfo.ulLastIndexTime = rec.timestamp;
                                pInt->audioIndexInfo.ulLastFileOffset = rec.offset;
                                pInt->audioIndexInfo.ulFirstIndex = 0;
                                pInt->audioIndexInfo.ulLastIndex = RM_PARSER_INDEX_TABLE_LEN -1;
                                pInt->audioIndexInfo.ulCurIndex = 0;
                                pInt->audioIndexInfo.ulNextIndexOffset = pInt->ulCurFileOffset + ulBytesToRead;
                            }
                            else
                            {
                                ulBytesToRead = hdr.num_recs*RM_PARSER_INDEX_RECORD_SIZE;
                                pInt->audioIndexInfo.pIndexBuffer = rm_parseri_malloc(pInt, ulBytesToRead);
                                //rm_parseri_error(pInt, pInt->audioIndexInfo.pIndexBuffer, "pInt->audioIndexInfo.pIndexBuffer");
                                if(HXNULL == pInt->audioIndexInfo.pIndexBuffer)
                                {
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                ulRet = pInt->fpRead(pInt->pUserRead, pInt->audioIndexInfo.pIndexBuffer, ulBytesToRead);
                                if(ulRet != ulBytesToRead)
                                {
                                    rm_parseri_free(pInt, pInt->audioIndexInfo.pIndexBuffer);
                                    retVal = HXR_FAIL;
                                    break;
                                }
                                //rm_parseri_error(pInt, hdr.num_recs, "hdr.num_recs");
                                pIndexBuf = pInt->audioIndexInfo.pIndexBuffer;
                                //rm_parseri_error(pInt, pIndex, "pIndex");
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->audioIndexInfo.ulNumIndexes = hdr.num_recs;
                                pInt->audioIndexInfo.ulFirstIndexTime = rec.timestamp;
                                pInt->audioIndexInfo.ulFIrstFileOffset = rec.offset;
                                //rm_parseri_error(pInt, &pIndex[hdr.num_recs -1], "&pIndex[hdr.num_recs -1]");
                                pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                                pInt->audioIndexInfo.ulLastIndexTime = rec.timestamp;
                                pInt->audioIndexInfo.ulLastFileOffset = rec.offset;
                                pInt->audioIndexInfo.ulFirstIndex = 0;
                                pInt->audioIndexInfo.ulLastIndex = hdr.num_recs -1;
                                pInt->audioIndexInfo.ulCurIndex = 0;
                                pInt->audioIndexInfo.ulNextIndexOffset = 0;
                            }
                            pInt->audioIndexInfo.bHaveIndex = TRUE;
                            rm_parseri_error(pInt, pInt->audioIndexInfo.ulFirstIndexTime, "ulFirstIndexTime");
                            rm_parseri_error(pInt, pInt->audioIndexInfo.ulFIrstFileOffset, "ulFIrstFileOffset");
                            rm_parseri_error(pInt, pInt->audioIndexInfo.ulLastIndexTime, "ulLastIndexTime");
                            rm_parseri_error(pInt, pInt->audioIndexInfo.ulLastFileOffset, "ulLastFileOffset");
                        }

                        ulIndexOffset = hdr.next_index_hdr;
                    }
                }
            }
        }

        rm_parseri_file_seek(pInt, ulCurFileOffset, HX_SEEK_ORIGIN_SET);
    }

    return retVal;
}

void rm_parseri_index_free(rm_parser_internal* pInt)
{

    if(pInt)
    {
        if(pInt->videoIndexInfo.pIndexBuffer)
        {
            rm_parseri_free(pInt, pInt->videoIndexInfo.pIndexBuffer);
        }
        if(pInt->audioIndexInfo.pIndexBuffer)
        {
            rm_parseri_free(pInt, pInt->audioIndexInfo.pIndexBuffer);
        }
    }
}

HX_RESULT rm_parseri_index_dynamic_load(rm_parser_internal* pInt, rm_index_info*pIndexInfo, BOOL bDirection)
{
    HX_RESULT retVal = HXR_FAIL;

    if(pInt&&pIndexInfo)
    {
        struct rm_index_rec rec;
        UINT32    ulBytesToRead = 0;
        UINT32    ulIndexToRead = 0;
        UINT32    ulRet = 0;
        BYTE*  pIndexBuf = HXNULL;
        UINT32    ulCurFileOffset = pInt->ulCurFileOffset;
        UINT32    ulReadOffset = 0;

        if(bDirection)
        {
            if(pIndexInfo->ulNextIndexOffset)
            {
                rm_parseri_file_seek(pInt, pIndexInfo->ulNextIndexOffset, HX_SEEK_ORIGIN_SET);
                ulIndexToRead = pIndexInfo->ulNumIndexesAll - pIndexInfo->ulLastIndex - 1;
                if(0 == ulIndexToRead)
                {
                    pInt->ulCurFileOffset = ulCurFileOffset;
                    return HXR_FAIL;
                }
                if(ulIndexToRead > RM_PARSER_INDEX_TABLE_LEN)
                {
                    ulBytesToRead = RM_PARSER_INDEX_TABLE_LEN*RM_PARSER_INDEX_RECORD_SIZE;
                    ulRet = pInt->fpRead(pInt->pUserRead, pIndexInfo->pIndexBuffer, ulBytesToRead);
                    if(ulRet != ulBytesToRead)
                    {
                        pInt->ulCurFileOffset = ulCurFileOffset;
                        return HXR_FAIL;
                    }
                    pIndexBuf = pIndexInfo->pIndexBuffer;
                    rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                    pIndexInfo->ulNumIndexes = RM_PARSER_INDEX_TABLE_LEN;
                    pIndexInfo->ulFirstIndexTime = rec.timestamp;
                    pIndexInfo->ulFIrstFileOffset = rec.offset;
                    pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                    rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                    pIndexInfo->ulLastIndexTime = rec.timestamp;
                    pIndexInfo->ulLastFileOffset = rec.offset;
                    pIndexInfo->ulFirstIndex += RM_PARSER_INDEX_TABLE_LEN;
                    pIndexInfo->ulLastIndex += RM_PARSER_INDEX_TABLE_LEN;
                    pIndexInfo->ulCurIndex = pIndexInfo->ulFirstIndex;
                    pIndexInfo->ulNextIndexOffset += ulBytesToRead;
                    pIndexInfo->ulCurIndexInBuf = 0;
                }
                else
                {
                    ulBytesToRead = ulIndexToRead*RM_PARSER_INDEX_RECORD_SIZE;
                    ulRet = pInt->fpRead(pInt->pUserRead, pIndexInfo->pIndexBuffer, ulBytesToRead);
                    if(ulRet != ulBytesToRead)
                    {
                        pInt->ulCurFileOffset = ulCurFileOffset;
                        return HXR_FAIL;
                    }
                    pIndexBuf = pIndexInfo->pIndexBuffer;
                    rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                    pIndexInfo->ulNumIndexes = ulIndexToRead;
                    pIndexInfo->ulFirstIndexTime = rec.timestamp;
                    pIndexInfo->ulFIrstFileOffset = rec.offset;
                    pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                    rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                    pIndexInfo->ulLastIndexTime = rec.timestamp;
                    pIndexInfo->ulLastFileOffset = rec.offset;
                    pIndexInfo->ulFirstIndex = pIndexInfo->ulLastIndex + 1;
                    pIndexInfo->ulLastIndex += ulIndexToRead;
                    pIndexInfo->ulCurIndex = pIndexInfo->ulFirstIndex;
                    pIndexInfo->ulNextIndexOffset = 0;
                    pIndexInfo->ulCurIndexInBuf = 0;
                }
                retVal = HXR_OK;
            }
        }
        else
        {
            if(pIndexInfo->ulFirstIndex)
            {
                ulReadOffset = pIndexInfo->ulIndexChunkOffset + (pIndexInfo->ulFirstIndex - RM_PARSER_INDEX_TABLE_LEN);
                rm_parseri_file_seek(pInt, ulReadOffset, HX_SEEK_ORIGIN_SET);
                ulBytesToRead = RM_PARSER_INDEX_TABLE_LEN*RM_PARSER_INDEX_RECORD_SIZE;
                ulRet = pInt->fpRead(pInt->pUserRead, pIndexInfo->pIndexBuffer, ulBytesToRead);
                if(ulRet != ulBytesToRead)
                {
                    pInt->ulCurFileOffset = ulCurFileOffset;
                    return HXR_FAIL;
                }
                pIndexBuf = pIndexInfo->pIndexBuffer;
                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                pIndexInfo->ulNumIndexes = RM_PARSER_INDEX_TABLE_LEN;
                pIndexInfo->ulFirstIndexTime = rec.timestamp;
                pIndexInfo->ulFIrstFileOffset = rec.offset;
                pIndexBuf += ulBytesToRead - RM_PARSER_INDEX_RECORD_SIZE;
                rm_parseri_unpack_index_rec_in_buffer(pIndexBuf, &rec);
                pIndexInfo->ulLastIndexTime = rec.timestamp;
                pIndexInfo->ulLastFileOffset = rec.offset;
                pIndexInfo->ulFirstIndex -= RM_PARSER_INDEX_TABLE_LEN;
                pIndexInfo->ulLastIndex -= RM_PARSER_INDEX_TABLE_LEN;
                pIndexInfo->ulCurIndex = pIndexInfo->ulFirstIndex;
                pIndexInfo->ulNextIndexOffset -= ulBytesToRead;
                pIndexInfo->ulCurIndexInBuf = 0;

                retVal = HXR_OK;
            }
        }

        pInt->ulCurFileOffset = ulCurFileOffset;
        
    }

    return retVal;
}

HX_RESULT rm_parseri_search_index_table(rm_parser_internal* pInt, rm_index_info* pIndexInfo, UINT32 ulSeekTime, UINT32 ulStreamNum,UINT32* pulFoundTime, UINT32* pulFoundOffset)
{
    HX_RESULT retVal = HXR_FAIL;

    if(pInt&&pIndexInfo&&pIndexInfo->ulNumIndexes)
    {
        struct rm_index_rec rec;
        UINT32 ulIndex = pIndexInfo->ulCurIndex - pIndexInfo->ulFirstIndex;
        UINT32 ulNumIdx = pIndexInfo->ulNumIndexes;
        BYTE*  pBuf =  pIndexInfo->pIndexBuffer+ ulIndex*RM_PARSER_INDEX_RECORD_SIZE;

        rm_parseri_error(pInt, ulIndex, "ulCurIndex");
        rm_parseri_error(pInt, ulNumIdx, "ulAllIndex");
        rm_parseri_unpack_index_rec_in_buffer(pBuf, &rec);
        while(rec.timestamp < ulSeekTime && ulIndex < ulNumIdx)
        {
            rm_parseri_error(pInt,rec.timestamp , "timestamp");
            rm_parseri_update_seek_table(pInt,
                                                                 ulStreamNum,
                                                                 rec.timestamp,
                                                                 rec.offset,
                                                                 HX_KEYFRAME_FLAG);
            ulIndex++;
            pIndexInfo->ulCurIndex++;
            pBuf += RM_PARSER_INDEX_RECORD_SIZE;
            rm_parseri_unpack_index_rec_in_buffer(pBuf, &rec);
        }
        rm_parseri_error(pInt, ulIndex, "ulIndex");
        rm_parseri_error(pInt, rec.timestamp, "find,timestamp");
        if(ulIndex < ulNumIdx)
        {
            rm_parseri_update_seek_table(pInt,
                                                                     ulStreamNum,
                                                                     rec.timestamp,
                                                                     rec.offset,
                                                                     HX_KEYFRAME_FLAG);
            *pulFoundTime = rec.timestamp;
            *pulFoundOffset = rec.offset;
            pIndexInfo->ulCurIndex++;
            pIndexInfo->ulCurIndexInBuf = ulIndex+1;
            pIndexInfo->ulFoundTime = rec.timestamp;
            retVal = HXR_OK;
        }
        else
        {
            pBuf -= RM_PARSER_INDEX_RECORD_SIZE;
            rm_parseri_unpack_index_rec_in_buffer(pBuf, &rec);
            *pulFoundTime = rec.timestamp;
            *pulFoundOffset = rec.offset;
            pIndexInfo->ulCurIndexInBuf = ulNumIdx+1;
            pIndexInfo->ulFoundTime = rec.timestamp;
            if(0 == pIndexInfo->ulNextIndexOffset)
            {
                retVal = HXR_OK;
            }
        }
    }

    return retVal;
}

HX_RESULT rm_parseri_search_index_in_buffer(rm_parser_internal* pInt, UINT32 ulSeekTime, UINT32 ulStreamNum)
{
    HX_RESULT retVal = HXR_FAIL;

    if (pInt && pInt->propHdr.index_offset && pInt->pStreamInfo &&
        pInt->ulNumStreams)
    {
        /* Initialize local variables */
        struct rm_stream_info* pInfo = &pInt->pStreamInfo[ulStreamNum];
        rm_index_info*  pIndexInfo = HXNULL;
        HX_RESULT ret = HXR_FAIL;

        pInfo->keyFramePacket.bValid = FALSE;
        
        if(ulStreamNum == pInt->ulVideoStreamNum)
        {
            pIndexInfo =  &(pInt->videoIndexInfo);
        }
        else if(ulStreamNum == pInt->ulAudioStreamNum)
        {
            pIndexInfo =  &(pInt->audioIndexInfo);
        }
        else
        {
            return retVal;
        }
        
        if(pIndexInfo->bHaveIndex)
        {
            rm_parseri_error(pInt, pIndexInfo->ulLastIndexTime, "ulLastIndexTime");
            if(ulSeekTime > pIndexInfo->ulLastIndexTime)
            {
                while(!HX_SUCCEEDED(retVal))
                {
                    retVal = rm_parseri_search_index_table(pInt, pIndexInfo, ulSeekTime, ulStreamNum,
                                                                                &pInfo->keyFramePacket.ulTimestamp, 
                                                                                &pInfo->keyFramePacket.ulFileOffset);
                    //pInfo->keyFramePacket.bValid = TRUE;
                    if(HXR_OK != retVal)
                    {
                        ret = rm_parseri_index_dynamic_load(pInt, pIndexInfo, TRUE);
                        if(HXR_OK != ret)
                        {
                            break;
                        }
                    }

                }
            }
            else
            {
                retVal = rm_parseri_search_index_table(pInt, pIndexInfo, ulSeekTime, ulStreamNum,
                                                                                &pInfo->keyFramePacket.ulTimestamp, 
                                                                                &pInfo->keyFramePacket.ulFileOffset);
                //pInfo->keyFramePacket.bValid = TRUE; 
            }
            pInfo->keyFramePacket.bValid = TRUE;
            retVal = HXR_OK;
        }
        
    }

    return retVal;

}

HX_RESULT rm_parseri_get_interval_from_seek_table(struct rm_seek_table* pTable, UINT32 ulTime, INT32* ulInterval)
{
    HX_RESULT retVal = HXR_FAIL;
    
    if(pTable&&ulInterval)
    {
        while(pTable->ulCurEntry < pTable->ulNumEntries && pTable->pEntry[pTable->ulCurEntry].ulTime < ulTime)
        {
            pTable->ulCurEntry++;
        }
        if(pTable->ulCurEntry < pTable->ulNumEntries)
        {
            *ulInterval = pTable->pEntry[pTable->ulCurEntry].ulTime - ulTime;
            retVal = HXR_OK;
        }
        else
        {
            pTable->ulCurEntry = pTable->ulNumEntries - 1;
            *ulInterval = -1;
        }
    }

    return retVal;
}
HX_RESULT rm_parseri_get_interval_from_index_table(rm_parser_internal* pInt, rm_index_info* pIndexInfo, UINT32 ulTime, INT32* ulInterval)
{
    HX_RESULT retVal = HXR_FAIL;

    if(pInt&&pIndexInfo&&ulInterval)
    {
        struct rm_index_rec rec;
        BYTE*  pBuf =  HXNULL;//pIndexInfo->pIndexBuffer+ (pIndexInfo->ulCurIndexInBuf)*RM_PARSER_INDEX_RECORD_SIZE;

        if(pIndexInfo->ulFoundTime == ulTime)
        {
            *ulInterval = 0;
            return HXR_OK;
        }

        pBuf =  pIndexInfo->pIndexBuffer+ (pIndexInfo->ulCurIndexInBuf)*RM_PARSER_INDEX_RECORD_SIZE;
        rm_parseri_unpack_index_rec_in_buffer(pBuf, &rec);
        while(pIndexInfo->ulCurIndexInBuf < pIndexInfo->ulNumIndexes && rec.timestamp < ulTime)
        {
            pIndexInfo->ulCurIndexInBuf++;
            pBuf =  pIndexInfo->pIndexBuffer+ (pIndexInfo->ulCurIndexInBuf)*RM_PARSER_INDEX_RECORD_SIZE;
            rm_parseri_unpack_index_rec_in_buffer(pBuf, &rec);
        }
        if(pIndexInfo->ulCurIndexInBuf < pIndexInfo->ulNumIndexes)
        {
            *ulInterval = rec.timestamp - ulTime;
            retVal = HXR_OK;
        }
        else
        {
            retVal = rm_parseri_index_dynamic_load(pInt, pIndexInfo, TRUE);
            if(HXR_OK == retVal)
            {
                *ulInterval = pIndexInfo->ulFirstIndexTime - ulTime;
                retVal = HXR_OK;
            }
            else
            {
                *ulInterval = -1;
            }
        }
        
       
    }

    return retVal;
}

INT32 rm_parseri_get_next_keyframe_interval(rm_parser_internal* pInt, UINT32 ulTime)
{
    HX_RESULT retVal = HXR_FAIL;
    INT32  ulInterval = -1;
    

    if(pInt)
    {
        struct rm_stream_info* pInfo =  HXNULL;
        
        pInfo = &pInt->pStreamInfo[pInt->ulVideoStreamNum];
              
        retVal = rm_parseri_get_interval_from_seek_table(&pInfo->seekTable, ulTime, &ulInterval);
        if(HXR_OK != retVal)
        {
            rm_parseri_get_interval_from_index_table(pInt, &pInt->videoIndexInfo, ulTime, &ulInterval);
        }
    
    }

    return ulInterval;
}
