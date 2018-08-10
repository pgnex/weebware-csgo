#pragma once
/*
	PURPOSE:
		All Objects Inherit This Class,
		Containing Methods All Objects Should Use,
		Such as Rendering, Updating and Handling Input
*/
struct basegraphical_t : public basepositioned_t
{
public:
	basegraphical_t() = default;
	virtual ~basegraphical_t() = default;

	virtual void render() = 0;
	virtual void update(deltatime_t delta) = 0;
	virtual bool input(input_type type, input_variable variable) = 0;

protected:

};