#pragma once
#ifndef APEMATRIX_H
#define APEMATRIX_H

#include "ApeFloat4.h"

namespace AML {
	struct APEMATRIX
	{
		union
		{
			float elements[4 * 4];
			APEFLOAT4 columns[4];
		};

		APEMATRIX();
		APEMATRIX(float diagonal);

		APEFLOAT4 GetColumn(int index)
		{
			index += 4;
			return APEFLOAT4(elements[index], elements[index + 1], elements[index + 2], elements[index + 3]);
		}

		static APEMATRIX Identity();
		static APEMATRIX Orthographic();
		static APEMATRIX Perspective();
	};
}
#endif