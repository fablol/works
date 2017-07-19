// website: http://EverET.org
#define BOOST_PYTHON_STATIC_LIB
#include <iostream>
#include <vector>
#include <boost/python.hpp>

class Map
{
public:
	Map() : m_map(10, std::vector<char>(20, '.'))
	{}

	void SetPixel(int x, int y, int val)
	{
		m_map[y][x] = val;
	}

	void Print()
	{
		for (unsigned int i = 0; i < m_map.size(); ++i)
		{
			for (unsigned int j = 0; j < m_map[i].size(); ++j)
			{
				std::cout << m_map[i][j];
			}
			std::cout << std::endl;
		}
	}

private:
	std::vector<std::vector<char> > m_map;
};

void InitPython()
{
	Py_Initialize();

	if(!Py_IsInitialized())
	{
		exit(-1);
	}
}

// Get the instance of the map, Singleton Pattern
// only one map instance exists
Map* GetMapInstance()
{
	static Map* the_map = NULL;
	if (!the_map)
	{
		the_map = new Map();
	}
	return the_map;
}

// export c++ function and class to python
BOOST_PYTHON_MODULE(MyEngine)
{
	using namespace boost::python;
	def("GetMapInstance", GetMapInstance,
		return_value_policy< reference_existing_object >());
	class_<Map>("Map", "Game Map")
		.def("Print", &Map::Print)
		.def("SetPixel", &Map::SetPixel,
		(arg("x"), "y", "val"));
}

int main()
{
	try
	{
		using namespace boost::python;

		InitPython();
		;
		//initMyEngine(); // init MyEngine Module

		// Add current path to sys.path. You have to
		// do this in linux. While in Windows,
		// current path is already in sys.path.
		object main_module = import( "__main__" );
		object main_namespace = main_module.attr( "__dict__" );
		object ignored = exec(
			"import sys\n"
			"sys.path.append('.')\n", main_namespace );

		Map* map = GetMapInstance();
		std::cout << "Before python\n";
		map->Print();

		// load python to design the map
		object mapMaker = import("mapmaker");
		object makeMap = mapMaker.attr("make_map");
		makeMap();

		std::cout << "\nAfter python\n";
		map->Print();
	}
	catch (...)
	{
		PyErr_Print();
	}

	return 0;
}