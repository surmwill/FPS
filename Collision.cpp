#include "Collision.h"

//todo fix all restrictive movement in collision

//sets all the intial values
Collision::Collision(GLfloat boundingMaxY, GLfloat boundingMinY, GLfloat boundingMaxX, GLfloat boundingMinX, GLfloat boundingMaxZ, GLfloat boundingMinZ, Camera * camera, glm::vec3 translate)
{
	this->boundingMaxX = boundingMaxX + offset + translate.x;
	this->boundingMinX = boundingMinX - offset + translate.x;
	this->boundingMaxY = boundingMaxY + offset + translate.y;
	this->boundingMinY = boundingMinY - offset + translate.y;
	this->boundingMaxZ = boundingMaxZ + offset + translate.z;
	this->boundingMinZ = boundingMinZ - offset + translate.z;

	ProcessCollisions(camera);
}


void Collision::ProcessCollisions(Camera * camera)
{


	//Check each dimension for collisions
	if (camera->FuturePosition.x >= boundingMinX && camera->FuturePosition.x <= boundingMaxX) hitX = true;
	if (camera->FuturePosition.y >= boundingMinY && camera->FuturePosition.y <= boundingMaxY) hitY = true;
	if (camera->FuturePosition.z >= boundingMinZ && camera->FuturePosition.z <= boundingMaxZ) hitZ = true;

	//if we are in all 3 dimensions of the bounding box we are in a collision
	if (hitX && hitY && hitZ) inCollision = true;
	else inCollision = false;

	//cout << inCollision << endl;
	//if hitx and prevHitx = false then add 1 to i when i is 3 that is our last one and keep doing the same direction push until we are no longer in that collision

	//cout << "hitx" <<  hitX << endl;
	//cout << "prevHitx" << prevHitX << endl;
	//cout << lastHit << endl;

	//cout << "x" << hitX << endl;
	//cout << "y" << hitY << endl;
	//cout << "z" << hitZ << endl;

	/*Looks at each dimension seperatly at first, based on flags set on each dimension we determine which dimension we hit last telling us which way we need to 
	"push" our character. For example we will be constantly hitting the x and y directions of the ground of our map so we don't want to lock up movment in the 
	x and z directions because we'd like to move about. However since we would be colliding with the y component of the map last we want to push up on our character
	*/

	//if we have a collision in the x direction
	if (hitX)
	{
		//and we have not previously hit the x direction before we increase our last hit variable, once last hit gets to 3 we know what are last dimension collision is
		if (!prevHitX)
		{
			lastHit++;
		}

		//if we are currently in a collision (3 dimensions) and it is our last hit dimension we set a flag to ensure this is the only dimension that gets frozen
		//and an unlock flag so we tell the camera that we neeed to constantly push up in this dimension
		if ((inCollision && lastHit == 3 && !flag) || unlockFlagX)
		{
			camera->hitX = 1;
			flag = true;
			unlockFlagX = true;
		}
	}
	//if we leave the x dimension collision we set our last hit counter down 1 as we no longer have a collision
	else if (!hitX && prevHitX) lastHit--;

	if (hitY)
	{
		if (!prevHitY) lastHit++;
		if ((inCollision && lastHit == 3 && !flag) || unlockFlagY)
		{
			camera->hitY = 1;
			flag = true;
			unlockFlagY = true;
		}
	}
	else if (!hitY && prevHitY) lastHit--;

	if (hitZ)
	{
		if (!prevHitZ) lastHit++;
		if ((inCollision && lastHit == 3 && !flag) || unlockFlagZ)
		{
			camera->hitZ = 1;
			cout << camera->hitZ << endl;
			flag = true;
			unlockFlagZ = true;
		}
	}
	else if (!hitZ && prevHitZ) lastHit--;

	prevHitX = hitX;
	prevHitZ = hitZ;
	prevHitY = hitY;

	//once we leave the 3 dimension collision we reset all the flags to get ready for another one
	if (prevCollision && !inCollision)
	{
		ResetFlags();
		flag = false;
		prevHitX = false;
		prevHitY = false;
		prevHitZ = false;
		unlockFlagX = false;
		unlockFlagY = false;
		unlockFlagZ = false;

		camera->hitX = 0;
		camera->hitY = 0;
		camera->hitZ = 0;

		//cout << "change in collision" << endl;
		lastHit = 0;
	}

	prevCollision = inCollision;

}


// creates a custom bouding box for testing
void Collision::CreateCustomBoundingBox(Camera * camera, char key)
{
	switch (key)
	{
	case ('r') : 
		boundingMaxX = camera->Position.x; 
		break;
	case ('f') : 
		boundingMinX = camera->Position.x; 
		break;
	case ('t') : 
		boundingMaxZ = camera->Position.z; 
		break;
	case ('g') : 
		boundingMinZ = camera->Position.z; 
		break;
	case ('y') : 
		boundingMaxY = camera->Position.y; 
		break;
	case ('h') : 
		boundingMinY = camera->Position.y; 
		break;
	case ('p') : flag2 = 1; break;
	
	default: break;

	};

	if (flag2 == 1)
	{
		this->ProcessCollisions(camera);
	}

	if (key == 'l')
	{
		cout << "maxX: " << boundingMaxX << endl;
		cout << "minX: " << boundingMinX << endl;
		cout << "maxY: " << boundingMaxY << endl;
		cout << "minY: " << boundingMinY << endl;
		cout << "maxZ: " << boundingMaxZ << endl;
		cout << "minZ: " << boundingMinZ << endl;
		flag2 = 0;
	}
}

//resets the flags
void Collision::ResetFlags()
{
	hitX = false;
	hitY = false;
	hitZ = false;


}

void Collision::WriteToFile()
{

	if (h == 0)
	{
		myfile.open("dimensions.txt");
		if (!myfile.is_open())
		{
			cout << "error opening file" << endl;
			return;
		}
		h = 1;
	}

	myfile << " min x" << boundingMinX << "\n";
	myfile << " max x" << boundingMaxX << "\n";
	myfile << " min y" << boundingMinY << "\n";
	myfile << " max y" << boundingMaxY << "\n";
	myfile << " min z" << boundingMinZ << "\n";
	myfile << " max z" << boundingMaxZ << "\n";
	myfile << "\n";

	if (closeFile == 1)
	{
		myfile.close();
		return;
	}
	else
	{
		cout << "hello" << endl;
	}

	
}
