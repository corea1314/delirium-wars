#ifndef _COMMAND_H
#define _COMMAND_H

class Command
{
public:
	virtual void Start() = 0;					// called before first execution
	virtual bool Execute() = 0;					// execute, returns true if finished
	virtual void Stop() = 0;					// called after last execution
	virtual void Pause( bool in_bPause ) = 0;	// called when command stack is paused/unpaused

	virtual Command* Create( const std::string& in_szName, const std::string& in_szScript ) = 0;	// used by the factory
};

class CommandStack
{
public:
	bool Push( Command* );
	void Update();

private:
	std::vector<Command*>	m_vecCommands;
};


class CommandMan
{
public:
	bool	Register( const std::string& in_szName, Command* in_pCommand );

	Command* Get( const std::string& in_szScript );

private:
	std::map< std::string, Command* >	m_mapCommand;
};

#endif//_COMMAND_H