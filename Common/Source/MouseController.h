/**
*
 MouseController
 Referenced: CMouseController by Toh Da Jun (Mar 2020)
 */
#ifndef MOUSE_H
#define MOUSE_H

class MouseController
{
public:
	static class MouseController* GetInstance(void);
	static void DestroyInstance(void);

	enum BUTTON_TYPE
	{
		LMB = 0,
		RMB,
		MMB,
		NUM_MB
	};
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_XOFFSET = 0,
		SCROLL_TYPE_YOFFSET,
		NUM_SCROLL_TYPE
	};

	// Update this class istance
	void UpdateMousePosition(const double _x, const double _y);
	void UpdateMouseButtonPressed(const int _slot);
	void UpdateMouseButtonReleased(const int _slot);
	void UpdateMouseScroll(const double WheelOffset_X, const double WheelOffset_Y);
	void PostUpdate(void);

	// Check the button and scroll wheel statuses
	bool IsButtonDown(const unsigned char _slot);
	bool IsButtonUp(const unsigned char _slot);
	bool IsButtonPressed(const unsigned char _slot);
	bool IsButtonReleased(const unsigned char _slot);
	double GetMouseScrollStatus(const SCROLL_TYPE _scrolltype) const;

	// Get values from this class istance
	double GetMousePositionX(void) const;
	double GetMousePositionY(void) const;
	double GetMouseDeltaX(void) const;
	double GetMouseDeltaY(void) const;

	// Get or Set status of keeping the mouse centered
	inline bool GetKeepMouseCentered() const { return bKeepMouseCentered; };
	inline void SetKeepMouseCentered(bool _value){ bKeepMouseCentered = _value; };

protected:
	// Constructor
	MouseController(void);
	// Destructor
	~MouseController(void);

	static MouseController* m_instance;

	double	curr_posX, curr_posY, 
			prev_posX, prev_posY, 
			delta_posX, delta_posY;
	unsigned char currBtnStatus, prevBtnStatus;
	double WheelOffset_X, WheelOffset_Y;

	// Boolean flag to indicate if the mouse will be kept centered
	bool bKeepMouseCentered;
	// Boolean flag to indicate that this class instance is waiting for its first update
	bool bFirstUpdate;
};

#endif
