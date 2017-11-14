#pragma once
class Boss
{
public:
	Boss();
	~Boss();

	Int2 GetPosition() { return m_Position; };
	Int2 SetPosition(Int2 position) { m_Position = position; };

private:
	Int2 m_Position;

};

