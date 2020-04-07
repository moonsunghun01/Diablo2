
#define TILESIZE 32 //오른쪽 화면 타일 사이즈
#define TILEX 20	//왼쪽화면 타일 사이즈 X
#define TILEY 20	//왼쪽 화면 타일 사이즈 Y

#define TILESIZEX TILEX * TILESIZE
#define TILESIZEY TILEY * TILESIZE

//화면 오른쪽 이미지 타일 갯수
#define SAMPLETILEX 20
#define SAMPLETILEY 9

//비트연산을 하기 위해 
#define ATTR_UNMOVABLE 0x00000001
#define ATTR_POSITION 0x00000002
//지형(EX 밟을때 사운드를 나게 하거나 이동할 수 없는 지역 등등)
enum TERRAIN
{
	TR_CEMENT, TR_DESERT, TR_GRASS, TR_WATER, TR_END
};

//오브젝트(EX 지형위쪽에 배치 움직이거나 부서지거나 하는 녀석 등등 변화를 줄수 있는 물체)
enum OBJECT
{
	OBJ_BLOCK,
	OBJ_BLOCK3,
	OBJ_BLOCKS,
	OBJ_TANK1,
	OBJ_TANK2,
	OBJ_NONE
};

//위치 좌표
enum POS
{
	POS_TANK1,
	POS_TANK2
};
//타일 구조체

struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rcTile;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};
//이미지 타일 구조체
struct tagSampleTile
{
	RECT rcTile[SAMPLETILEX* SAMPLETILEY];
	int terrainFrameX;
	int terrainFrameY;
};
//현재 타일 구조체
struct tagCurrentTile
{
	int x;
	int y;
};

struct tagOBJAttribute
{
	int strengh;		//내구도
};
