# C++ ClothSim
---

## Synopsis
---

<img src="images/inrun.gif" width="1080">

A particle-based cloth simulation for C++ assignment using NCCA ngl library with Qt Creator.  
The simulation happens in real time.  
As university ngl library is included, the code can't run on other equipments, but just an example.  
Source codes can be foude in folder **`src`**.  
Header files can be foude in folder **`include`**.


---

## Key Features
---

1. Dynamic Parameters
    - Shear/Bend Resistance, Stiffness Coefficient
    - Damping Coefficient
    - Wind Force on z axis
   
2. Hanging Options
    - Can choose which corner(s) to hang the cloth in the air.

3. Collide
    - Enable/Disable sollision with the ground plane / the collision sphere.
   
4. Reset
    - Reset the camera, the sphere and cloth to the initial position.

5. Suspend / Play
    - Stop simulation and replay simulation.
    
6. Mouse & Key Control
   - *`Alt + LeftMouseButton`* for camera rotate.
   - *`Alt + RightMouseButton`* for camera translation.
   - *`wheel`* for zoom.
   - *`LeftMouseButton drag`* for moving the collision sphere.
   
---

## Usage
---

1. To Download/Clone
   - You can download the folder or clone with https://github.com/ywen19/clothSim.git .
2. Setup
   - Use QtCreator or other building environment to configure the project. Make sure the linking to NGL library is working.
   - Turn off shadow build.
   - Turn off run in terminal(optional) and turn off run->Image path(when no image library) is on machine.
   - Build and Run
3. Manual Instruction
   - See from **`Key Feature`**.
---

Motivation & Background
---

Particle-based cloth simulation under mass-spring algorithm was chosen for the project as it includes physics, collisions and use of strings. For better visulisation, all calculations respond to parameter changes in real time. A set of libraries and classes were built in order to have a clearer structure and hierarchy.

---


## Design & Algorithm
---
   - User Interface
     I chose QOpenGLWidget as the base class for NGLScene as UI and OpenGL widget were both needed. A structure to hold cursor positions had a copy in NGLScene to achieve mouse control.
     
   - Class: Particle, Spring, Triangle
     Particle class stored attributes as inedx, mass, force, position and damping coeeficient for each particle. *F=ma* was used for calculating acceleration and *position += (position-previous_position)(1.f-damping) + pAccelaration*  was used to update the particle. Method *getTotalForce(Vec3 _Force)* adds new force into the particle attribute.
     
     Spring was for calculating spring forces. The attribute `force` would be cleared to zero before a new spring force was calculated. The force is added to both two given particles to avoid re-calculating when the pair(a,b) switch to (b,a).
     
     Triangle was used to get a triangular surface between three neighboring particles and the normal for the surface for wind force. Unfortunately, there was not enough time left to use it for surfacing particles and rendering out the cloth as a polygon surface.
     ```
     #force calculation ( pseudo code )
     for each particle (p1) in the cloth
     {
        #p1's neighbor p2 (direct/secondary neighboring)
        if (p1 and p2 are both on cloth grid)
        {
            p1 += spring_force;
            p2 -= spring_force;
        }
     
        Force += gravity;
        Force += wind;
        if (collision = enable) { Force += collision_Force; }
     }
     ```
     
     - Class: Cloth
       A one-dimension vector was created to store all particles for the simulation. All other classes were instanced in this class for calculating and updating. The main loop through all particles was within this class. The update() method was called by the TimerEvent inside NGLScene.
       





