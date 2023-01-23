#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
class DLMSsetting
{
	unsigned char m_SenderFrame;
	unsigned char m_ReceiverFrame;

public:
	unsigned char getSenderFrame()
	{
		return m_SenderFrame;
	}

	unsigned char getReceiverFrame()
	{
		return m_ReceiverFrame;
	}

	void ResetFrameSequence()
	{
		m_SenderFrame = 0x10;  // CLIENT_START_SENDER_FRAME_SEQUENCE;
		m_ReceiverFrame = 0xE; // CLIENT_START_RCEIVER_FRAME_SEQUENCE;
	}

	// Increase receiver sequence.
	//
	// @param value
	//            Frame value.
	// Increased receiver frame sequence.
	static unsigned char IncreaseReceiverSequence(unsigned char value)
	{
		return ((value + 0x20) | 0x10 | (value & 0xE));
	}

	// Increase sender sequence.
	//
	// @param value
	//            Frame value.
	// Increased sender frame sequence.
	static unsigned char IncreaseSendSequence(unsigned char value)
	{
		return (unsigned char)((value & 0xF0) | ((value + 0x2) & 0xE));
	}

	bool CheckFrame(unsigned char frame)
	{
		// If notify
		if (frame == 0x13)
		{
			return true;
		}
		// If U frame.
		if ((frame & 0x3) == 3)
		{
			if (frame == 0x73 || frame == 0x93)
			{
				ResetFrameSequence();
			}
			return true;
		}
		// If S -frame
		if ((frame & 0x3) == 1)
		{
			m_ReceiverFrame = IncreaseReceiverSequence(m_ReceiverFrame);
			return true;
		}

		// Handle I-frame.
		unsigned char expected;
		if ((m_SenderFrame & 0x1) == 0)
		{
			expected = IncreaseReceiverSequence(IncreaseSendSequence(m_ReceiverFrame));
			if (frame == expected)
			{
				m_ReceiverFrame = frame;
				return true;
			}
		}
		// If answ er for RR.
		else
		{
			expected = IncreaseSendSequence(m_ReceiverFrame);
			if (frame == expected)
			{
				m_ReceiverFrame = frame;
				return true;
			}
		}
		return false;
	}

	unsigned char GetNextSend(unsigned char first)
	{
		if (first)
		{
			m_SenderFrame = IncreaseReceiverSequence(IncreaseSendSequence(m_SenderFrame));
		}
		else
		{
			m_SenderFrame = IncreaseSendSequence(m_SenderFrame);
		}
		return m_SenderFrame;
	}
};
DLMSsetting setting1;

__declspec(dllexport) int ControlFrame(unsigned char frame,
									   unsigned char &sendsequence, unsigned char &receivesequence)
{
	int value = setting1.CheckFrame(frame);
	sendsequence = setting1.getSenderFrame();
	receivesequence = setting1.getReceiverFrame();
	return value;
}

__declspec(dllexport) unsigned char Getnextsenderframe(int i)
{
	return setting1.GetNextSend(i);
}

int main()
{
	unsigned char s_number = 0x1f;
	unsigned char r_number = 0x3d;
	ControlFrame(0x93, s_number, r_number);
	cout << hex << (int)s_number << " " << (int)r_number << endl;

	ControlFrame(0x30, s_number, r_number);
	cout << hex << (int)s_number << " " << (int)r_number << endl;

	cout << (int)setting1.GetNextSend(1) << endl;

	ControlFrame(0x52, s_number, r_number);

	cout << (int)setting1.GetNextSend(1) << endl;

	ControlFrame(0x74, s_number, r_number);

	cout << (int)setting1.GetNextSend(1) << endl;

	/*ControlFrame(setting1, 0x10, s_number, r_number);
	cout << hex << (int)s_number << " " << (int)r_number << endl;*/

	/*ControlFrame(setting1, 0x31, s_number, r_number);
	cout << hex << (int)s_number << " " << (int)r_number << endl;*/
	/*s_number=setting1.GetNextSend(1);
	cout << (int)s_number << endl;

	cout << sizeof(s_number) << endl;*/
	return 0;
}