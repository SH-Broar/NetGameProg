#include "stdafx.h"
#include "Scene.h"
#include "Framework.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

CScene::CScene(SceneTag tag, CFramework * pFramework)
{
	m_Tag = tag; 
	m_pFramework = pFramework;
}
