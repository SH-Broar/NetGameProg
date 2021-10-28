#pragma once
//#include "Object.h"

class CFramework;
class CObject;

class CScene
{
public: 
	// CScene::SceneTag:: �������� �ҷ��� �ܿ� �ʿ䵵 define�� �ʿ䵵 ���� ���� ���빰�� �� �� �ִ�. �� ������ϸ� include ����.
	enum SceneTag {
		Title
		, Main_Lobby
		, Control_Tip
		, Select_Char
		, Ingame
		, Scoreboard
		, count
	};
public:
	CScene();
	CScene(SceneTag tag, CFramework* pFramework);
	virtual ~CScene();	// CScene�� ��ӹ��� Ÿ��Ʋ���� �Ҹ��ڵ� ~CScene();�� �ȴ�. �׷� Ÿ��Ʋ���� �Ҹ��� �� �� �Ҹ��ڵ� �޾ƿ´�.
						// �̹� �Ҹ��ڰ� �ִµ� Ÿ��Ʋ���� ���������� �Ű�Ἥ ���� �ʿ䰡 ����. ����, virtual�� �ٿ� ��ӹ޾� �Ҹ��Ų��.
						// virtual Ÿ�� ���� Ŭ������ ������ �ö� ����, ���� �Ҹ��ڸ� ���� ȣ���ؼ� ��ӹ��� Ŭ������ �ѹ��� �Ҹ��Ű�� ���̴�.

	// ���� �ѹ� ���� std::vector�� ó���ϴ� ���Ͱ� �� �������� 10���� �̻� �Ѿ�� ���� �ȵȴ�.
	// vector�� ������ �迭�̴�.
	// for(auto& p : list) p = &arr // ������� for��

	// �����Լ��� ����ƽ����̶� ������ ���� ������ ���� �� �� �ִ�. �����Լ� ���ο� �����Լ��� ������ �ȴ�.

	virtual bool OnCreate() = 0;
	virtual void BuildObjects() = 0;
	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render(HDC hdc) = 0;
protected:
	SceneTag m_Tag;
	CFramework* m_pFramework;	// ���ڷ� �޾ƿ��°� �ƴ϶� �����Ҵ��� �ؿ´�.
};