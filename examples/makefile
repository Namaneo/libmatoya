name = 0-minimal

BIN  = example.exe
OBJ  = $(name).obj
LIBS = ../bin/windows/%Platform%/matoya.lib

CFLAGS = -I../src/

!IFDEF DEBUG
CFLAGS   = $(CFLAGS) /Oy- /Ob0 /Zi -DCC_DEBUG
LD_FLAGS = /debug
!ENDIF

SYS_LIBS = \
	shlwapi.lib \
	shell32.lib \
	user32.lib \
	userenv.lib \
	ws2_32.lib \
	imm32.lib \
	advapi32.lib \
	dxguid.lib \
	winmm.lib \
	gdi32.lib \
	bcrypt.lib \
	secur32.lib \
	crypt32.lib \
	opengl32.lib \
	xinput9_1_0.lib \
	windowscodecs.lib \
	d3d9.lib \
	d3d11.lib \
	hid.lib \
	ole32.lib \
	winhttp.lib

all: clean app

clean:
	-taskkill /F /IM $(BIN)
	del $(BIN)

clear:
	cls

app: $(OBJ)
	pushd .. & nmake
	link /out:$(BIN) $(OBJ) $(LIBS) $(SYS_LIBS) $(LD_FLAGS)
