#include <ApePCH.h>

#define _MAINRUN
#ifdef _MAINRUN
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int CmdShow)
{
	std::shared_ptr<System> m_pSystem;

	m_pSystem = std::make_shared<System>();

	if (m_pSystem)
	{
		m_pSystem->Run();
	}

	m_pSystem.reset();

	return 0;
}
#endif

#ifdef _TESTRUN

#include "ApePCH.h"
#include <iostream>
class DataSetter
{
public:
	DataSetter() {}
	~DataSetter() {}

	typedef std::vector<uint8_t> BufferType;
	BufferType m_Data;

	size_t m_ElementSize;
	size_t m_Offset;
	size_t m_NumElements;

	void SetData(void* data, size_t elementSize, size_t offset, size_t numElements);
	float GetData() const;
};
int main()
{
	struct FloatTypes
	{
		FloatTypes()
			: x(1), y(2), z(3)
		{}
		int x = 1;
		int y = 2;
		int z = 3;
	};

	FloatTypes* floats = new FloatTypes();

	DataSetter* dataSetting = new DataSetter;

	uint8_t a = 3;
	uint8_t b = 5;
	uint8_t c = 29;

	std::vector<uint8_t> numbers;
	numbers.push_back(a);
	numbers.push_back(b);
	numbers.push_back(c);

	dataSetting->SetData(numbers.data(), sizeof(uint8_t), 0, 3);

	std::cout << (UINT)dataSetting->m_Data.at(0) << std::endl;
	std::cout << floats->x << std::endl;

	numbers.clear();
	delete floats;
	delete dataSetting;
	system("PAUSE");
	return 0;
}

void DataSetter::SetData(void* data, size_t elementSize, size_t offset, size_t numElements)
{
	m_ElementSize = elementSize;
	m_Offset = offset;
	m_NumElements = numElements;

	unsigned char* first = (unsigned char*)data + (offset * numElements);
	unsigned char* last = first + (numElements * elementSize);
	m_Data.assign(first, last);
}

float DataSetter::GetData() const
{
	return 1.0f;
}
#endif