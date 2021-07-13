/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     platform_list.h
  * @brief    Head file for list structure related.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-9-28
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PLATFORM_LIST_
#define _PLATFORM_LIST_

#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup Platform_List
  * @{
  */

/** @defgroup Platform_List_Exported_Types Exported Types
  * @{
  */

/** @brief general element type of link list */
typedef struct _plt_list_e_t
{
    struct _plt_list_e_t *pnext;
    uint8_t data[4];
} plt_list_e_t;

/** @brief general list type of link list */
typedef struct
{
    plt_list_e_t *pfirst;
    plt_list_e_t *plast;
    uint32_t count;
} plt_list_t;

/** @} */

/** @defgroup Platform_List_Exported_Functions Exported Functions
  * @brief
  * @{
  */

/**
  * @brief push in the element to the end of the list
  * @param[in] plist: the list pointer
  * @param[in] plist_e: the element pointer
  * @return none
  */
void  plt_list_push(plt_list_t *plist, void *plist_e);

/**
  * @brief pop out the first element of the list
  * @param[in] plist: the list pointer
  * @return the list element
  */
void *plt_list_pop(plt_list_t *plist);

/**
  * @brief insert the element to any position of the list
  * @param[in] plist: the list pointer
  * @param[in] plist_e: the ahead element pointer
  * @param[in] plist_e_new: the new element pointer
  * @return none
  */
void  plt_list_insert(plt_list_t *plist, void *plist_e, void *plist_e_new);

/**
  * @brief delete the element at any position of the list
  * @param[in] plist: the list pointer
  * @param[in] plist_e_previous: the ahead element pointer
  * @param[in] plist_e: the deleted element pointer
  * @return none
  */
void  plt_list_delete(plt_list_t *plist, void *plist_e_previous, void *plist_e);

/** @} */
/** @} */

END_DECLS

#endif /* _PLATFORM_LIST_ */
