void DrawMessageWindow();
void BlankMessageWindow();
//void ScrollMessageWindowUp(byte lines);
void WriteLineMessageWindow(char message[], byte delay);
extern char Messages[64][4];
void DrawCharStats();