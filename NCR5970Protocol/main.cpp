#include <hidapi.h>
#include <string.h>

hid_device* handle;

bool initialize_display()
{
	unsigned char buf[65];
	memset(&buf, 0, 65);

  hid_init();

	handle = hid_open(0x404, 0x335, NULL);

	if (!handle)
		return false;

	// Send signal to enable display
	buf[0] = 0x00;
	buf[1] = 0x02;
	buf[2] = 0x1B;
	buf[3] = 0x01;

	hid_write(handle, buf, 65);

	// Send back response code

	buf[0] = 0x00;
	buf[1] = 0x07;
	buf[2] = 0x1B;
	buf[3] = 0x20;
	buf[4] = 0x1B;
	buf[5] = 0x05;
	buf[6] = 0x1B;
	buf[7] = 0x17;
	buf[8] = 0x05;

	hid_write(handle, buf, 65);

	memset(&buf, 0, 65);

	// enable?

	buf[0] = 0x00;
	buf[1] = 0x02;
	buf[2] = 0x1B;
	buf[3] = 0x0C;

	hid_write(handle, buf, 65);

	return true;
}

void display_message(const char* message)
{
	unsigned char buf[65];
	memset(&buf, 0, 65);

	char header[4] = { 0x00, 0x2A, 0x1B, 0x02 };
	memcpy(buf, header, 4);

	memcpy(buf + 4, message, strlen(message));

	hid_write(handle, buf, 65);
}

int main(int argc, char* argv[])
{
	if (!initialize_display()) {
		return -1;
	}

	display_message("Hello world");
	
	hid_close(handle);
  
	return 0;
}