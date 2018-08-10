#pragma once

/*
	PURPOSE:
		Handles All Relationships From Perants to Children.
*/
struct baserelational_t : public basegraphical_t
{
public:
	virtual ~baserelational_t();

	virtual void render() override;
	virtual void update(deltatime_t delta) override;
	virtual bool input(input_type type, input_variable variable) override;

	void addchild(basegraphical_t*);

protected:
	std::vector<basegraphical_t*> m_children;

};
