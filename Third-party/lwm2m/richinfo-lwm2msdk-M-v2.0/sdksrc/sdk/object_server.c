
#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _server_instance_
{
    struct _server_instance_ * next;   // matches lwm2m_list_t::next
    uint16_t    instanceId;            // matches lwm2m_list_t::id
    uint16_t    shortServerId;
    uint32_t    lifetime;
    bool        storing;
    char        binding[4];
} server_instance_t;

static uint8_t prv_get_value(lwm2m_data_t * dataP,
                             server_instance_t * targetP)
{
    switch (dataP->id)
    {
    case LWM2M_SERVER_SHORT_ID_ID:
        lwm2m_data_encode_int(targetP->shortServerId, dataP);
        return COAP_205_CONTENT;

    case LWM2M_SERVER_LIFETIME_ID:
        lwm2m_data_encode_int(targetP->lifetime, dataP);
        return COAP_205_CONTENT;

    case LWM2M_SERVER_DISABLE_ID:
        return COAP_405_METHOD_NOT_ALLOWED;

    case LWM2M_SERVER_STORING_ID:
        lwm2m_data_encode_bool(targetP->storing, dataP);
        return COAP_205_CONTENT;

    case LWM2M_SERVER_BINDING_ID:
        lwm2m_data_encode_string(targetP->binding, dataP);
        return COAP_205_CONTENT;

    case LWM2M_SERVER_UPDATE_ID:
        return COAP_405_METHOD_NOT_ALLOWED;

    default:
        return COAP_404_NOT_FOUND;
    }
}

static uint8_t prv_server_read(uint16_t instanceId,
                               int * numDataP,
                               lwm2m_data_t ** dataArrayP,
                               lwm2m_object_t * objectP)
{
    server_instance_t * targetP;
    uint8_t result;
    int i;

    targetP = (server_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP) return COAP_404_NOT_FOUND;

    // is the server asking for the full instance ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
                LWM2M_SERVER_SHORT_ID_ID,
                LWM2M_SERVER_LIFETIME_ID,
                LWM2M_SERVER_STORING_ID,
                LWM2M_SERVER_BINDING_ID
        };
        int nbRes = sizeof(resList)/sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0 ; i < nbRes ; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do
    {
        result = prv_get_value((*dataArrayP) + i, targetP);
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}

static uint8_t prv_server_discover(uint16_t instanceId,
                                   int * numDataP,
                                   lwm2m_data_t ** dataArrayP,
                                   lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    result = COAP_205_CONTENT;

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
            LWM2M_SERVER_SHORT_ID_ID,
            LWM2M_SERVER_LIFETIME_ID,
            LWM2M_SERVER_MIN_PERIOD_ID,
            LWM2M_SERVER_MAX_PERIOD_ID,
            LWM2M_SERVER_DISABLE_ID,
            LWM2M_SERVER_TIMEOUT_ID,
            LWM2M_SERVER_STORING_ID,
            LWM2M_SERVER_BINDING_ID,
            LWM2M_SERVER_UPDATE_ID
        };
        int nbRes = sizeof(resList)/sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0 ; i < nbRes ; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }
    else
    {
        for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++)
        {
            switch ((*dataArrayP)[i].id)
            {
            case LWM2M_SERVER_SHORT_ID_ID:
            case LWM2M_SERVER_LIFETIME_ID:
            case LWM2M_SERVER_MIN_PERIOD_ID:
            case LWM2M_SERVER_MAX_PERIOD_ID:
            case LWM2M_SERVER_DISABLE_ID:
            case LWM2M_SERVER_TIMEOUT_ID:
            case LWM2M_SERVER_STORING_ID:
            case LWM2M_SERVER_BINDING_ID:
            case LWM2M_SERVER_UPDATE_ID:
                break;
            default:
                result = COAP_404_NOT_FOUND;
            }
        }
    }

    return result;
}

static uint8_t prv_set_int_value(lwm2m_data_t * dataArray,
                                 uint32_t * data)
{
    uint8_t result;
    int64_t value;

    if (1 == lwm2m_data_decode_int(dataArray, &value))
    {
        if (value >= 0 && value <= 0xFFFFFFFF)
        {
            *data = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_406_NOT_ACCEPTABLE;
        }
    }
    else
    {
        result = COAP_400_BAD_REQUEST;
    }
    return result;
}

static uint8_t prv_server_write(uint16_t instanceId,
                                int numData,
                                lwm2m_data_t * dataArray,
                                lwm2m_object_t * objectP)
{
    server_instance_t * targetP;
    int i;
    uint8_t result;

    targetP = (server_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
    {
        return COAP_404_NOT_FOUND;
    }

    i = 0;
    do
    {
        switch (dataArray[i].id)
        {
        case LWM2M_SERVER_SHORT_ID_ID:
        {
            uint32_t value;

            result = prv_set_int_value(dataArray + i, &value);
            if (COAP_204_CHANGED == result)
            {
                if (0 < value && value <= 0xFFFF)
                {
                    targetP->shortServerId = value;
                }
                else
                {
                    result = COAP_406_NOT_ACCEPTABLE;
                }
            }
        }
        break;

        case LWM2M_SERVER_LIFETIME_ID:
            result = prv_set_int_value(dataArray + i, (uint32_t *)&(targetP->lifetime));
            break;

        case LWM2M_SERVER_DISABLE_ID:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;

        case LWM2M_SERVER_STORING_ID:
        {
            bool value;

            if (1 == lwm2m_data_decode_bool(dataArray + i, &value))
            {
                targetP->storing = value;
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
        }
        break;

        case LWM2M_SERVER_BINDING_ID:
            if ((dataArray[i].type == LWM2M_TYPE_STRING || dataArray[i].type == LWM2M_TYPE_OPAQUE)
             && dataArray[i].value.asBuffer.length > 0 && dataArray[i].value.asBuffer.length <= 3
             && (strncmp((char*)dataArray[i].value.asBuffer.buffer, "U",   dataArray[i].value.asBuffer.length) == 0
              || strncmp((char*)dataArray[i].value.asBuffer.buffer, "UQ",  dataArray[i].value.asBuffer.length) == 0
              || strncmp((char*)dataArray[i].value.asBuffer.buffer, "S",   dataArray[i].value.asBuffer.length) == 0
              || strncmp((char*)dataArray[i].value.asBuffer.buffer, "SQ",  dataArray[i].value.asBuffer.length) == 0
              || strncmp((char*)dataArray[i].value.asBuffer.buffer, "US",  dataArray[i].value.asBuffer.length) == 0
              || strncmp((char*)dataArray[i].value.asBuffer.buffer, "UQS", dataArray[i].value.asBuffer.length) == 0))
            {
                strncpy(targetP->binding, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        case LWM2M_SERVER_UPDATE_ID:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;

        default:
            return COAP_404_NOT_FOUND;
        }
        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}

static uint8_t prv_server_execute(uint16_t instanceId,
                                  uint16_t resourceId,
                                  uint8_t * buffer,
                                  int length,
                                  lwm2m_object_t * objectP)

{
    server_instance_t * targetP;

    targetP = (server_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP) return COAP_404_NOT_FOUND;

    switch (resourceId)
    {
    case LWM2M_SERVER_DISABLE_ID:
        // executed in core, if COAP_204_CHANGED is returned
        return COAP_204_CHANGED;

    case LWM2M_SERVER_UPDATE_ID:
        // executed in core, if COAP_204_CHANGED is returned
        return COAP_204_CHANGED;

    default:
        return COAP_405_METHOD_NOT_ALLOWED;
    }
}

static uint8_t prv_server_delete(uint16_t id,
                                 lwm2m_object_t * objectP)
{
    server_instance_t * serverInstance;

    objectP->instanceList = lwm2m_list_remove(objectP->instanceList, id, (lwm2m_list_t **)&serverInstance);
    if (NULL == serverInstance) return COAP_404_NOT_FOUND;

    lwm2m_free(serverInstance);

    return COAP_202_DELETED;
}

static uint8_t prv_server_create(uint16_t instanceId,
                                 int numData,
                                 lwm2m_data_t * dataArray,
                                 lwm2m_object_t * objectP)
{
    server_instance_t * serverInstance;
    uint8_t result;

    serverInstance = (server_instance_t *)lwm2m_malloc(sizeof(server_instance_t));
    if (NULL == serverInstance) return COAP_500_INTERNAL_SERVER_ERROR;
    memset(serverInstance, 0, sizeof(server_instance_t));

    serverInstance->instanceId = instanceId;
    objectP->instanceList = LWM2M_LIST_ADD(objectP->instanceList, serverInstance);

    result = prv_server_write(instanceId, numData, dataArray, objectP);

    if (result != COAP_204_CHANGED)
    {
        (void)prv_server_delete(instanceId, objectP);
    }
    else
    {
        result = COAP_201_CREATED;
    }

    return result;
}

//lwm2m_object_t * get_server_object()
lwm2m_object_t * get_server_object(int serverId,int lifetime)
{
    lwm2m_object_t * serverObj;

    serverObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != serverObj)
    {
        server_instance_t * serverInstance;

        memset(serverObj, 0, sizeof(lwm2m_object_t));

        serverObj->objID = 1;

        // Manually create an hardcoded server
        serverInstance = (server_instance_t *)lwm2m_malloc(sizeof(server_instance_t));
        if (NULL == serverInstance)
        {
            lwm2m_free(serverObj);
            return NULL;
        }

        memset(serverInstance, 0, sizeof(server_instance_t));
        serverInstance->instanceId = 0;
        serverInstance->shortServerId = serverId;
        serverInstance->lifetime = lifetime;
        serverInstance->storing = false;
        serverInstance->binding[0] = 'U';
        serverObj->instanceList = LWM2M_LIST_ADD(serverObj->instanceList, serverInstance);

        serverObj->readFunc = prv_server_read;
        serverObj->writeFunc = prv_server_write;
        serverObj->createFunc = prv_server_create;
        serverObj->deleteFunc = prv_server_delete;
        serverObj->executeFunc = prv_server_execute;
        serverObj->discoverFunc = prv_server_discover;
    }

    return serverObj;
}

void free_server_object(lwm2m_object_t * object)
{
    while (object->instanceList != NULL)
    {
        server_instance_t * serverInstance = (server_instance_t *)object->instanceList;
        object->instanceList = object->instanceList->next;
        lwm2m_free(serverInstance);
    }
    lwm2m_free(object);
}
