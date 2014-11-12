#pragma once

#include <sstream>

#include "../Graphics/GraphicsSystem.h"
#include "../Utils/Event.h"

/*
*	Event<const unsigned int&> scoreChanged
*
*	Score(tgui::Gui& gui, const glm::vec2 position)
*	~Score()
*
*	void setScore(const unsigned int& score)
*	inline unsigned int getScore()
*/

class Score
{
	private:
	
		unsigned int score;
		tgui::Gui& gui;
		tgui::Label::Ptr label;
		
	public:
	
		Event<const unsigned int&> eventScoreChanged;
	
		Score(tgui::Gui& gui, const glm::vec2 position) : score(0), gui(gui), label(gui)
		{
			label->setText("0");
			label->setPosition(position.x, position.y);
		} //Score
		
		~Score()
		{
			gui.remove(label);
		} //~Score
		
		void setScore(const unsigned int& score)
		{
			if (score == this->score)
			{
				return;
			} //if
		
			this->score = score;
			
			stringstream ss;
			ss << score;
			label->setText(ss.str());
			
			eventScoreChanged.exec(score);
		} //setScore
		
		inline unsigned int getScore() { return score; }
}; //Score
