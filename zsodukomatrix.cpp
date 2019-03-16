#include "zsodukomatrix.hpp"

#include <iostream>

#include "stageunit.hpp"


ZSodukoMatrix::ZSodukoMatrix()
{
	// 为棋盘格分配内存
	m_stage.resize(10, std::vector<uint16_t>(10, 0));

	// 生成数字和标记的相互转换表·
	extern std::unordered_map<uint16_t, uint16_t> z_flag_to_num;
	extern std::vector<uint16_t> z_num_to_flag;
	z_num_to_flag.resize(10, 0);
	for (uint16_t i = 1; i < z_num_to_flag.size(); ++i)
	{
		z_num_to_flag[i] = 1 << i;
	}
	for (uint16_t i = 1; i < z_num_to_flag.size(); ++i)
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
	// 统计当前 Stage 中的空格
	std::queue< StagePosition > to_solve_queue;
	CountSpace(to_solve_queue);

	// 为避免死循环使用变量以记录队列中元素是否变少
	int current_queue_size = to_solve_queue.size();
	int processed_count = 0;

	int solved_count = 0;

	// 开始遍历
	while (processed_count++ < current_queue_size)
	{
		StagePosition current_space = to_solve_queue.front();
		to_solve_queue.pop();


		uint16_t res = TryToFillSingleSpace(current_space.m_x, current_space.m_y);

		if (res == 0)
			to_solve_queue.push(current_space);
		else
		{
			m_stage[current_space.m_y][current_space.m_x] = res;
			solved_count++;
			std::cout << "Step " << solved_count << " : "  << current_space.m_x << "  " << current_space.m_y << "---" << res << std::endl;

			current_queue_size = to_solve_queue.size();
			processed_count = 0;
		}

		// 根据已记录的数字来判断是否再次进行遍历
		if (processed_count == current_queue_size)
		{
			break;
		}
	}

	std::cout << "solved count: " << solved_count << std::endl;
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

	// i 代表第几行
	for (int i = 1; i < 10; ++i)
	{
		// j 代表第几列
		for (int j = 1; j < 10; ++j)
		{
			// 计算当前元素在对应的网格单元下标及其在对应网格单元内的下标
			uint16_t grid_unit_index;
			uint16_t grid_unit_data_index;
			PositionMod3(j, i, grid_unit_index, grid_unit_data_index);

			// 取得当前元素的指针
			uint16_t* data = &m_stage[i][j];

			// 将当前元素的指针设置到其对应的各个单元之中
			m_row_units[i].SetData(j, data);
			m_col_units[j].SetData(i, data);
			m_grid_units[grid_unit_index].SetData(grid_unit_data_index, data);


		}
	}
}

void ZSodukoMatrix::CountSpace(std::queue<StagePosition> & space_queue)
{
	for (uint16_t i = 1; i < 10; ++i)
	{
		for (uint16_t j = 1; j < 10; ++j)
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
	uint16_t grid_unit_index, grid_unit_data_index;
	PositionMod3(x, y, grid_unit_index, grid_unit_data_index);
	uint16_t row_space_flag, row_existing_flag;
	uint16_t col_space_flag, col_existing_flag;
	uint16_t grid_space_flag, grid_existing_flag;
	GetSpaceFlagAndExistingFlag(
		x, y,
		row_space_flag, row_existing_flag,
		col_space_flag, col_existing_flag,
		grid_space_flag, grid_existing_flag);

	// 1，当前位置的可选项应为 行、列、网格单元所缺失的数字集合的交集
	uint16_t available_num_flag = (~row_existing_flag) & (~col_existing_flag) & (~grid_existing_flag) & 1022;
	if (available_num_flag == 0)	// 若该可选项flag为空则说明之前的填写有误
		return available_num_flag;
	uint16_t available_num = ConverFlagToNum(available_num_flag);
	if (available_num != 0)	// 此时available_num为空说明求得的available_num_flag不是2的整数次幂，可用项大于1
		return available_num;

	// 2，若某数字在当前网格可用，但在当前行/列/网格的其它空白处均不可用，则该数字必为当前空白处的可填写数字
	// 统计当前网格所在行/列/网格上的其余空白位置
	std::vector<uint16_t> row_spaces;
	std::vector<uint16_t> col_spaces;
	std::vector<uint16_t> grid_spaces;
	extern std::vector<uint16_t> z_num_to_flag;
	row_space_flag &= ~z_num_to_flag[y];
	col_space_flag &= ~z_num_to_flag[x];
	grid_space_flag &= ~z_num_to_flag[grid_unit_data_index];
	ConvertFlagToVector(row_space_flag, row_spaces);
	ConvertFlagToVector(col_space_flag, col_spaces);
	ConvertFlagToVector(grid_space_flag, grid_spaces);

	uint16_t other_row_space_flag, other_row_existing_flag;
	uint16_t other_col_space_flag, other_col_existing_flag;
	uint16_t other_grid_space_flag, other_grid_existing_flag;
	uint16_t other_available_flag, temp_available_num_flag, other_available_num;

	other_available_flag = 0;
	temp_available_num_flag = available_num_flag;
	for (uint16_t i = 0; i < row_spaces.size(); ++i)
	{
		GetSpaceFlagAndExistingFlag(
			row_spaces[i], y,
			other_row_space_flag, other_row_existing_flag,
			other_col_space_flag, other_col_existing_flag,
			other_grid_space_flag, other_grid_existing_flag
		);
		other_available_flag = (~other_row_existing_flag) & (~other_col_existing_flag) & (~other_grid_existing_flag) & 1022;
		temp_available_num_flag &= ~other_available_flag;
	}
	other_available_num = ConverFlagToNum(temp_available_num_flag);
	if (other_available_num != 0)
		return other_available_num;

	other_available_flag = 0;
	temp_available_num_flag = available_num_flag;
	for (uint16_t i = 0; i < col_spaces.size(); ++i)
	{
		GetSpaceFlagAndExistingFlag(
			x, col_spaces[i],
			other_row_space_flag, other_row_existing_flag,
			other_col_space_flag, other_col_existing_flag,
			other_grid_space_flag, other_grid_existing_flag
		);
		other_available_flag = (~other_row_existing_flag) & (~other_col_existing_flag) & (~other_grid_existing_flag) & 1022;
		temp_available_num_flag &= ~other_available_flag;
	}
	other_available_num = ConverFlagToNum(temp_available_num_flag);
	if (other_available_num != 0)
		return other_available_num;

	other_available_flag = 0;
	temp_available_num_flag = available_num_flag;
	uint16_t other_x, other_y;
	for (uint16_t i = 0; i < grid_spaces.size(); ++i)
	{
		PositionMod3Reverse(grid_unit_index, grid_spaces[i], other_x, other_y);
		GetSpaceFlagAndExistingFlag(
			other_x, other_y,
			other_row_space_flag, other_row_existing_flag,
			other_col_space_flag, other_col_existing_flag,
			other_grid_space_flag, other_grid_existing_flag
		);
		other_available_flag = (~other_row_existing_flag) & (~other_col_existing_flag) & (~other_grid_existing_flag) & 1022;
		temp_available_num_flag &= ~other_available_flag;
	}
	other_available_num = ConverFlagToNum(temp_available_num_flag);
	if (other_available_num != 0)
		return other_available_num;

	return 0;
}

void ZSodukoMatrix::PositionMod3(uint16_t x, uint16_t y, uint16_t& grid_unit_index, uint16_t& grid_unit_data_index)
{
	uint16_t grid_x = (x - 1) / 3;
	uint16_t data_x = (x - 1) % 3;
	uint16_t grid_y = (y - 1) / 3;
	uint16_t data_y = (y - 1) % 3;
	grid_unit_index = grid_y * 3 + grid_x;
	grid_unit_data_index = data_y * 3 + data_x + 1;
}

void ZSodukoMatrix::PositionMod3Reverse(uint16_t grid_unit_index, uint16_t grid_unit_data_index, uint16_t& x, uint16_t& y)
{
	uint16_t grid_x, grid_y, data_x, data_y;

	grid_y = grid_unit_index / 3;
	grid_x = grid_unit_index % 3;

	grid_unit_data_index -= 1;
	data_y = grid_unit_data_index / 3;
	data_x = grid_unit_data_index % 3;

	x = grid_x * 3 + data_x + 1;
	y = grid_y * 3 + data_y + 1;
}

void ZSodukoMatrix::GetSpaceFlagAndExistingFlag(
	uint16_t x, uint16_t y, 
	uint16_t& row_space_flag, uint16_t& row_existing_flag, 
	uint16_t& col_space_flag, uint16_t& col_existing_flag, 
	uint16_t& grid_space_flag, uint16_t& grid_existing_flag)
{

	uint16_t grid_unit_index;
	uint16_t grid_unit_data_index;
	PositionMod3(x, y, grid_unit_index, grid_unit_data_index);

	m_row_units[y].GetSpacePositionAndDataFlag(row_space_flag, row_existing_flag);
	m_col_units[x].GetSpacePositionAndDataFlag(col_space_flag, col_existing_flag);
	m_grid_units[grid_unit_index].GetSpacePositionAndDataFlag(grid_space_flag, grid_existing_flag);
}

uint16_t ZSodukoMatrix::ConverFlagToNum(uint16_t flag)
{

	if (flag != 0 && (flag & flag - 1) == 0)
	{
		extern std::unordered_map<uint16_t, uint16_t> z_flag_to_num;
		return z_flag_to_num[flag];
	}
	return 0;
}

void ZSodukoMatrix::ConvertFlagToVector(uint16_t flag, std::vector<uint16_t>& nums)
{
	nums.clear();
	extern std::vector<uint16_t> z_num_to_flag;
	for (uint16_t i = 0; i < z_num_to_flag.size(); ++i)
	{
		if (z_num_to_flag[i] & flag)
			nums.push_back(i);
	}
}
