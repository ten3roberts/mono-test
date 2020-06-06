using System;
using System.Runtime.CompilerServices;

class Script
{
	string name = "noname";
	vec3 position;

	public Script()
	{
		Application.Log("Using default constructor for script");
		position = new vec3(1,2,3);
	}

	public Script(string name)
	{
		this.name = name;
		Application.Log("Created script with name: " + name);
	}

	public vec3 Update(int frame)
	{
		Application.Log("Updating script '" + name + "' with frame: " + frame);

		position.x += MathF.Sin(position.x);
		return position;
	}

}

class Application
{
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	extern static string Sample();

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public extern static void Log(string msg);

	static void Main()
	{
		Int64 a = 0;
		for (int i = 0; i < 10000000; i++)
		{
			a += i;
		}
		Sample();
		Log("Hello from C#");
	}


	static void Update(int frame)
	{
		Log("Static update frame: " + frame);
	}

}