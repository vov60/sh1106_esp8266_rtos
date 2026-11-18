ESP32 U8G2 library support
There is an excellent open source library called u8g2 that can be found on Github here:
https://github.com/olikraus/u8g2

There is an excellent open source library called u8g2 that can be found on Github here:

https://github.com/olikraus/u8g2


Note: For I2C, after calling the display specific setup, invoke:

u8x8_SetI2CAddress(&u8g2.u8x8,<address>);
Note that <address> is the I2C address already shifted left to include the read/write flag. For example, if you are using an SH1106, instead 
of supplying the address 0x3C which would b supply 0x78(0x3c<<1) which would be .



Compiling U8G2
To use the actual U8g2 library in  project:

Create a directory called components in project directory.
Change into the components directory.
Run git clone https://github.com/olikraus/u8g2.git to bring in a the latest copy of u8g2 library.
Change into the u8g2 directory.
Create a file called component.mk
Enter the following in the component.mk file:
COMPONENT_SRCDIRS:=csrc
COMPONENT_ADD_INCLUDEDIRS:=csrc


