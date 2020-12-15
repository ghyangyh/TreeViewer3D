#ifndef GL_LOGGER_H
#define GL_LOGGER_H

/*
 * This file contains various logger utilities to
 * help detect errors in the opengl program.
 * Author: Yinhui Yang
 * Zhejiang A&F University
*/


#include <cstdarg>

bool restart_gl_log(const char* aLogFileName);
bool gl_log(const char* aLogFileName, const char* aMessage, ...);
bool gl_log_err(const char* aLogFileName, const char* aMessage, ...);
void log_gl_params(const char* aLogFileName);


#endif
