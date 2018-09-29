myid  =999999;

NPC_SPIDER = 0;
NPC_NAGA_GUARD = 1;
NPC_ROCK_WARRIOR = 2;
NPC_TREE_GUARD = 3;
NPC_TURTLE = 4;
NPC_DRAGON = 5;

function set_myid(x)
 myid =x; 
end

function LoadMonsterData(npcType)
if(npcType == NPC_SPIDER) then
my_x = math.random(85,95)
my_z = math.random(20,50)
my_hp = 30;
my_dmg = 3;
my_exp = 10;
elseif(npcType == NPC_NAGA_GUARD) then
my_x = math.random(110,148)
my_z = math.random(62,80)
my_hp = 50;
my_dmg = 5;
my_exp = 20;
elseif(npcType == NPC_ROCK_WARRIOR) then
my_x = math.random(143,143)
my_z = math.random(363,363)
my_hp = 400;
my_dmg = 15;
my_exp = 200;
elseif(npcType == NPC_TREE_GUARD) then
my_x = math.random(138,149)
my_z = math.random(131,331)
my_hp = 100;
my_dmg = 8;
my_exp = 30;
elseif(npcType == NPC_TURTLE) then
my_x = math.random(11,75)
my_z = math.random(-7,8)
my_hp = 10;
my_dmg = 2;
my_exp = 5;
end
return my_x, my_z, my_hp, my_dmg, my_exp
end