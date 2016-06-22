/////////////////////////////////////////////
// Filename: Font.cpp
/////////////////////////////////////////////
#include "apeFont.h"

apeFont::apeFont()
{
	m_Font = 0;
	m_FontTexture = 0;
}

apeFont::apeFont(const apeFont& other)
{

}

apeFont::~apeFont()
{

}

bool apeFont::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, float fontHeight, int spaceSize)
{
	m_fontHeight = fontHeight;
	m_spaceSize = spaceSize;

	// Load in the text file containing the font data.
	if (!LoadFontData(fontFilename))
		return false;

	return true;
}

void apeFont::Shutdown()
{
	ReleaseFontData();

	return;
}

bool apeFont::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType3[95];
	if (!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void apeFont::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

	return;
}

void apeFont::BuildVertexArray(void* vertice, char* sentence, float drawX, float drawY)
{
	VertexType2* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType2*)vertice;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for (i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + (float)m_spaceSize;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].mPos = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].mTexture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].mPos = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].mTexture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].mPos = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f);  // Bottom left.
			vertexPtr[index].mTexture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].mPos = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].mTexture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].mPos = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].mTexture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].mPos = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].mTexture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}

int apeFont::GetSentencePixelLength(char* sentence)
{
	int pixelLength, numLetters, i, letter;

	pixelLength = 0;
	numLetters = (int)strlen(sentence);

	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i] - 32);

		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += (m_Font[letter].size + 1);
		}
	}

	return pixelLength;
}

int apeFont::GetFontHeight()
{
	return (int)m_fontHeight;
}