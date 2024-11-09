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
/*const char *GetDebugSeverityStr(VkDebugUtilsMessageSeverityFlagBitsEXT Severity) {
	switch (Severity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			return "SEVERITY VERBOSE BIT";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			return "Info";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			return "WARNING";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			return "SEVERITY ERROR BIT";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			return "SEVERITY FLAG BITS MAX ENUM";
		default:
			printf("Invalide severity code %d\n", Severity);
		exit(1);
	}
	return "No Such Serverity!";
}

const char *GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type) {
	switch (Type) {
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			return "GENERAL_BIT_EXT";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			return "VALIDATION_BIT_EXT";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			return "PERFORMANCE BIT EXT";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
			return "DEVICE ADDRESS BINDING BIT EXT";
		default:
			printf("Invalid type code %d\n", Type);
		exit(1);
	}
	return "No SUSH TYPE!";
}*/

#endif //GAMEENGINE_BK_VULKAN_UTIL_H
