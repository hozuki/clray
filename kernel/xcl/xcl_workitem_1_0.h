//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_XCL_WORKITEM_1_0_H
#define CLRAY_XCL_WORKITEM_1_0_H

#include "xcl_types.h"

extern uint get_work_dim();
extern size_t get_global_size(uint dimIndex);
extern size_t get_global_id(uint dimIndex);
extern size_t get_local_size(uint dimIndex);
extern size_t get_local_id(uint dimIndex);
extern size_t get_num_groups(uint dimIndex);
extern size_t get_group_id(uint dimIndex);

#endif //CLRAY_XCL_WORKITEM_1_0_H
