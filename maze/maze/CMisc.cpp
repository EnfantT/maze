#include <iostream>

#include "glad/glad.h"

#include "CMisc.h"

using namespace std;

bool CMisc::m_ver3_1 = false;

CMisc::CMisc(void)
{
}

CMisc::~CMisc(void)
{
}

bool CMisc::IsGLVersion_3_1(void)
{
	return m_ver3_1;
}

void CMisc::EnableVersion_3_1(void)
{
	cout << "Version 3.1" << endl;
	CMisc::m_ver3_1 = true;
}

/* End of a file */
