/**
 KeyboardController
 Referenced: CKeyboardController by Toh Da Jun (Mar 2020)
 */
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <bitset>
#include <windows.h>

class KeyboardController
{
public:
	static KeyboardController* GetInstance(void);
	static void DestroyInstance(void);

	// Constant to indicate how many keys will be processed
	const static int MAX_KEYS = 348;

	// Perform pre-update operations
	void PreUpdate(void);

	// Perform update operation for a key
	void Update(const int key, const int action);

	// Perform post-update operations
	void PostUpdate(void);

	// User Interface
	bool IsKeyDown(const int key);
	bool IsKeyUp(const int key);
	bool IsKeyPressed(const int key);
	bool IsKeyReleased(const int key);

	// Reset a key
	void ResetKey(const int key);
	// Reset all keys
	void Reset(void);

private:
	KeyboardController(void);
	~KeyboardController(void);

	static KeyboardController* m_instance;

	// Bitset to store information about current and previous keypress statuses
	std::bitset<MAX_KEYS> currStatus, prevStatus;
};
#endif

