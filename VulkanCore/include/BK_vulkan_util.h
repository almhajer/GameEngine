//
// Created by mac on 06/11/2024.
//

#ifndef GAMEENGINE_BK_VULKAN_UTIL_H
#define GAMEENGINE_BK_VULKAN_UTIL_H
#define CHECK_VK_RESULT(res, msg) \
	if (res != VK_SUCCESS) {      \
		fprintf(stderr, "Error in %s:%d - %s, code %x\n", __FILE__, __LINE__, msg, res);  \
		exit(1);	\
	}


#endif //GAMEENGINE_BK_VULKAN_UTIL_H
