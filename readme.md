# GameDevelop

> Farenew

这个内容就是学习做游戏的一些demo代码. 主要学习的知识来源是Ben, 差不多就是抄了他的代码2333, 他在Youtube上的频道里有他录制的内容, 链接:

[MakingGamesWithBen](https://www.youtube.com/channel/UCL5m1_llmeiAdZMo_ZanIvg)

此外, [这个系列](http://www.opengl-tutorial.org/)的教程也讲授了OpenGL的基本内容. 收获颇多. 

---

要自己编译运行的话, 这个目录已经是工作目录了, 直接下载visual studio, 加载solution. 在右侧项目列表里, 右键ZombieGame project. 设置ZombieGame为`startup project`, progject dependency设置为`Bengine`, 模式设置为`debug`下的`x86`模式即可. 

--- 

去年的时候其实已经写了一大半了, 但因为种种事项没有完全实现. 这里重新把后面的内容补上, 并且添加一些记录. 后续有空的话, 会继续开发, 添加声音, 关卡变化, 购买商店, 更好的贴图, 动作等. 由于这里已经实现了基础的框架了, 后面继续开发不会很困难了. 

这个部分录制了一个小视频, 视频链接:

https://youtu.be/sjjozG7CHSo


---

游戏里主要改进就是改进了移动机制, 原来的实现贴着墙走可能会"卡"在墙上, 例如贴着右边的墙, 同时按下"右"和"下", 那么可能判定为首先和下面的tile碰撞, 此时不允许玩家向下, 所以就"卡"在墙上了. 

---

整个游戏调用OpenGL的基础指令, 实现了一个名为Bengine的引擎, 引擎里包括Camera, Shader的处理, InputManager读取贴图, SpriteBatch实现与OpenGL的交互, 做内容的绘制. 

基于引擎, 实现了ZombieGame, 游戏的逻辑也很简单, 一些主要的逻辑和流程有:

1. `MainGame`

这里存放了游戏运行的主要内容, 构造的时候会初始化一些参数, 包括分辨率, FPS等. 

然后调用`maingame.run`进入游戏循环. 


`void MainGame::run()`

```C++
void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}
```


2. `initSystems`

```C++
void MainGame::initSystems() {
    // 初始化SDL
	Bengine::init();

    // 创建Window
	_window.create("ZombieGame", _screenWidth, _screenHeight, 0);

    // 设置背景色
	glClearColor(0.6f, 0.6f, 0.6f, 0.1f);

    // 初始化Shaders, 包括编译, 添加attribute, 链接. 
	initShaders();

    // 初始化camera, 根据分辨率建立ortho matrix, 可以通过这个矩阵normalize全局坐标
	_camera.init(_screenWidth, _screenHeight);

    // 建立spriteBatch, 用来绘制内容, 具体绘制的方式
	_agentSpriteBatch.init();
}
```

3.  `initLevel()`

构造human和player, 把它们放在对应位置上. human放在了随机位置上. 

```C++
void MainGame::initLevel() {
	char level = _curLevel + 1 + '0';
	std::string filePath = "Levels/level" + std::string{ level } + ".txt";
	_levels.push_back(new Level(filePath));

	_player = new Player();
	Bengine::Color PlayerColor(0,0,180,255);

	_player->init(PLAYER_SPEED, _levels[_curLevel]->getPlayerPos(), PlayerColor, &_inputManager);

	_humans.push_back(_player);

	// random generate to generate humans
	std::mt19937 rand;
	rand.seed(time(nullptr));
	// here distribution not starting from 0 or 1 since we want to avoid generate human at the border, which may cause detection out of vector subscript limit
	std::uniform_real_distribution<float> randX(3, _levels[_curLevel]->getWidth() - 3);
	std::uniform_real_distribution<float> randY(3, _levels[_curLevel]->getHeight() - 3);

	// add all random humans
	for (int i = 0; i < _levels[_curLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(rand) * TILE_WIDTH, randY(rand) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	// add all zombies
	const std::vector<glm::ivec2>& zombiePositions = _levels[_curLevel]->getZombiePos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	_levels[_curLevel]->RenderLevelObject();
}
```

level在构造的时候, 已经做了大量处理, 构造函数里会读取文件内容, 然后设置spritebatch把图像数据发送到显卡上. 因此这些内容是不被更改的. 当游戏里`draw`的时候, 直接`render`一下level的数据即可. 

4. `gameLoop()`

设置fps. 然后进入循环, 持续处理游戏:

- 检测当前FPS
- 处理输入, 通过`_inputManager`发送对应的输入数据
- `updateAgents`. 更新所有agents的状态, 包括human和zombies. 游戏逻辑和输入会改变对应的agent状态. 这里会更新它们的位置, 状态等. 
- 更新相机位置, 以player的位置设立相机. 
- 绘制游戏. 把agents加入到spritebatch中. 


5. `updateAgents`

这里会处理agent之间的碰撞, 位置的变化. 并且在zombie和human碰撞后, 会删掉这个human, 加入一个zombie. 

这里会调用每个object自己的update. 里面定义了object的行为. 

这里实现的碰撞是AABB碰撞: [What is AABB - Collision detection?](https://stackoverflow.com/a/22512393/6769374)

此外, 武器与世界, 武器与对象的碰撞都是简单的球形碰撞. 

6. 武器逻辑

使用`bullets`来存放需要绘制的子弹. player会存一个`gun`和`bullets`的vector, 可以给player添加`gun`. 指定了武器的参数. 

然后在更新player的时候, 如果当前鼠标左键被按下, 那么以鼠标方向, 调用`Gun::triggerFire`, 在其中会按照firerate调用`Gun::fire`, 在这里会在`bullet`里添加内容. 以当前player的位置和鼠标方向, 添加bullets. 每个bullets的方向和速度等都是确定不变的, 除了和player相关的参数, 速度等和武器有关. 

接下来, 在`maingame`流程里, 会更新bullets, 即更新bullets的位置. 然后在drawGame`里, 会把bullets放到spritebatch里绘制. 