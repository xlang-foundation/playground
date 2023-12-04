import DrawSystem as ds

ds.CreateTimer(1,1000)
obj1 = ds.DrawObj(10,10,100,100,"XLang")

obj2 = ds.DrawObj(400,100,100,100,"Remoting")

pos = 10
ds.OnTimer += (timerId){
	extern pos;
	ds.UpdateObj(obj1,10+pos,10);
	ds.UpdateObj(obj2,400,100+pos);
	pos += 10;
}
print("test")