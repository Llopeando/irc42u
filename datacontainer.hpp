namespace dc{

enum Type{
	eVector,
	eList,
	nDeque
}

struct Input{
	Type type;
	uint32_t size;
	Input *pnext;
}


class DataContainer{
	DataContainer(Input *input);
	~DataContainer();

	private:
		std::map<Type, void *> data;
}

}