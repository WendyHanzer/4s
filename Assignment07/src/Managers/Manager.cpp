Manager::Manager(Engine* engine, std::type_info type)
{
	if (engine)
	{
		onDestroyAction = engine->eventDestroy.reg([this]() { delete this; });

		this->type = engine->addManager(type, this);

		eventDestroy.reverse();
	} //if
	else
	{
		onDestroyAction = NULL;
	} //else

} //Manager

Manager::~Manager()
{
	if (onDestroyAction)
	{
		delete onDestroyAction;

		eventDestroy.exec();
	} //if
} //~Manager