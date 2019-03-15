#include "zsodukomatrix.hpp"

#include <iostream>

#include "stageunit.hpp"


ZSodukoMatrix::ZSodukoMatrix()
{
	// Ϊ���̸�����ڴ�
	m_stage.resize(10, std::vector<uint16_t>(10, 0));

	// �������ֺͱ�ǵ��໥ת����
	extern std::unordered_map<uint16_t, uint16_t> z_flag_to_num;
	extern std::vector<uint16_t> z_num_to_flag;
	z_num_to_flag.resize(10, 0);
	for (size_t i = 1; i < z_num_to_flag.size(); ++i)
	{
		z_num_to_flag[i] = 1 << i;
	}
	for (size_t i = 1; i < z_num_to_flag.size(); ++i)
	{
		z_flag_to_num[z_num_to_flag[i]] = i;
	}
}

ZSodukoMatrix::~ZSodukoMatrix()
{
	
}

void ZSodukoMatrix::SetData(std::vector<uint16_t> data)
{
	if (data.size() < 81)
		return;

	size_t data_index = 0;
	for (size_t i = 1; i < 10; ++i)
	{
		for (size_t j = 1; j < 10; ++j)
		{
			m_stage[i][j] = data[data_index++];
		}
	}

	InitUnits();
}

void ZSodukoMatrix::SolveSoduko()
{
	// ͳ�Ƶ�ǰ Stage �еĿո�
	std::queue< StagePosition > to_solve_queue;
	CountSpace(to_solve_queue);

	// Ϊ������ѭ��ʹ�ñ����Լ�¼������Ԫ���Ƿ����
	int current_queue_size = to_solve_queue.size();
	int processed_count = 0;

	// ��ʼ����
	while (processed_count++ < current_queue_size)
	{
		StagePosition current_space = to_solve_queue.front();
		to_solve_queue.pop();


		uint16_t res = TryToFillSingleSpace(current_space.m_x, current_space.m_y);

		if (res == 0)
			to_solve_queue.push(current_space);
		else
			m_stage[current_space.m_y][current_space.m_x] = res;

		// �����Ѽ�¼���������ж��Ƿ��ٴν��б���
		if (processed_count == current_queue_size)
		{
			if (current_queue_size == to_solve_queue.size())
				break;
			else
			{
				current_queue_size = to_solve_queue.size();
				processed_count = 0;
			}
		}
	}
}

void ZSodukoMatrix::PrintToConsole()
{
	for (size_t i = 1; i < 10; ++i)
	{
		for (size_t j = 1; j < 10; ++j)
		{
			std::cout << m_stage[i][j];
			if (j == 3 || j == 6)
				std::cout << " | ";
			else
				std::cout << "   ";
		}

		if (i == 3 || i == 6)
			std::cout << std::endl << "__________|___________|__________" << std::endl << "          |           |" << std::endl;
		else
			std::cout << std::endl << "          |           |" << std::endl;
	}
}

void ZSodukoMatrix::InitUnits()
{
	m_row_units.resize(10, StageUnit(ROW));
	m_col_units.resize(10, StageUnit(COL));
	m_grid_units.resize(9, StageUnit(GRID));

	// i ����ڼ���
	for (int i = 1; i < 10; ++i)
	{
		// j ����ڼ���
		for (int j = 1; j < 10; ++j)
		{
			// ���㵱ǰԪ���ڶ�Ӧ������Ԫ�±꼰���ڶ�Ӧ����Ԫ�ڵ��±�
			int grid_unit_index;
			int grid_unit_data_index;
			PositionMod3(j, i, grid_unit_index, grid_unit_data_index);

			// ȡ�õ�ǰԪ�ص�ָ��
			uint16_t* data = &m_stage[i][j];

			// ����ǰԪ�ص�ָ�����õ����Ӧ�ĸ�����Ԫ֮��
			m_row_units[i].SetData(j, data);
			m_col_units[j].SetData(i, data);
			m_grid_units[grid_unit_index].SetData(grid_unit_data_index, data);


		}
	}
}

void ZSodukoMatrix::CountSpace(std::queue<StagePosition> & space_queue)
{
	for (size_t i = 1; i < 10; ++i)
	{
		for (size_t j = 1; j < 10; ++j)
		{
			if ( m_stage[i][j] == 0 )
			{
				space_queue.push(StagePosition(j, i));
			}
		}
	}
}

uint16_t ZSodukoMatrix::TryToFillSingleSpace(uint16_t x, uint16_t y)
{
	
	int grid_unit_index;
	int grid_unit_data_index;
	PositionMod3(x, y, grid_unit_index, grid_unit_data_index);

	uint16_t row_space, row_existing_flag;
	m_row_units[y].GetSpacePositionAndDataFlag(row_space, row_existing_flag);

	uint16_t col_space, col_existing_flag;
	m_col_units[x].GetSpacePositionAndDataFlag(col_space, col_existing_flag);

	uint16_t grid_space, grid_existing_flag;
	m_grid_units[grid_unit_index].GetSpacePositionAndDataFlag(grid_space, grid_existing_flag);

	uint16_t res = (~row_existing_flag) & (~col_existing_flag) & (~grid_existing_flag) & 1022;
	if (res == 0)
		return res;
	
	if ((res & res - 1) == 0)
	{
		extern std::unordered_map<uint16_t, uint16_t> z_flag_to_num;
		return z_flag_to_num[res];
	}

	return 0;
}

void ZSodukoMatrix::PositionMod3(uint16_t x, uint16_t y, int& grid_unit_index, int& grid_unit_data_index)
{
	int grid_x = (x - 1) / 3;
	int data_x = (x - 1) % 3;
	int grid_y = (y - 1) / 3;
	int data_y = (y - 1) % 3;
	grid_unit_index = grid_y * 3 + grid_x;
	grid_unit_data_index = data_y * 3 + data_x + 1;
}
