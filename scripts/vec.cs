struct vec3
{
	public float x, y, z;
	public vec3(int x, int y, int z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
		Application.Log("Created new vec3");
	}
}