#pragma once
#if !defined(__CMISC_H)
#define __CMISC_H

#define StatusPrint() do { GLenum status; status = glGetError(); if (status != GL_NO_ERROR) cerr << __func__ << ":" << __LINE__ << ":" << status << endl; } while (0)

#endif
/* end of a file */
