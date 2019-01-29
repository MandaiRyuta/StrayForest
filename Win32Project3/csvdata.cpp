#include "csvdata.h"


bool CsvConnect::CsvRead(const std::string & filename, std::vector<std::vector<std::string>>& table, const char delimiter)
{
	// �t�@�C�����J��
	std::fstream filestream(filename);
	if (!filestream.is_open())
	{
		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		return false;
	}

	// �t�@�C����ǂݍ���
	while (!filestream.eof())
	{
		// �P�s�ǂݍ���
		std::string buffer;
		filestream >> buffer;

		// �t�@�C������ǂݍ��񂾂P�s�̕��������؂蕶���ŕ����ă��X�g�ɒǉ�����
		std::vector<std::string> record;              // �P�s���̕�����̃��X�g
		std::istringstream streambuffer(buffer); // ������X�g���[��
		std::string token;                       // �P�Z�����̕�����
		while (getline(streambuffer, token, delimiter))
		{
			// �P�Z�����̕���������X�g�ɒǉ�����
			record.push_back(token);
		}

		// �P�s���̕�������o�͈����̃��X�g�ɒǉ�����
		table.push_back(record);
	}

	return true;
}

bool CsvConnect::CsvWrite(const std::string & filename, std::vector<std::vector<std::string>>& table, const char delimiter)
{
	return true;
}
