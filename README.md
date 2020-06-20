# ClothSim
===

## Synopsis
---


A particle-based cloth simulation for C++ assignment using NCCA ngl library. The simulation happens in real time.

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
   - You can download the folder or clone with https://github.com/NCCA/cfgaa2019-Ywen219 .
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

## Research
---

I combined the looping method, the strength calculations and particle structure mainly from these three sources and made changes accordingly.

1. Initial Research
   - At the initial stage, I looked up the book *'Computer Animation: Algorithms and Techniques'* by Rick Parent as I wanted to do a particle simulation with bullet physics. Later on, I realized that cloth simulation was a better choice to have physics and strings at the same time. I kept particle attributes for updating and claculation from the sample structure in the book.
   
2. Spring Force Calculation
   
   - A dissertation called 'Implementation of Cloth Simulation that can be used in Games and Animation' by Navneeth Nagaraj, B.Eng. from https://scss.tcd.ie/publications/theses/diss/2016/TCD-SCSS-DISSERTATION-2016-054.pdf was helpful enough to explain all forces needed for the simulation. It showed formulars to calculate spring stregth and also wind force. 
   
3. Loop through Particles

   - A very useful tutorial was from https://viscomp.alexandra.dk/?p=147 by Jesper Mosegaard. I learnt how to loop through all particles to calculte spring forces in a more economic way. However, using his loop with mass-spring system resulted in a bias move. So I made changes accordingly for mass-spring model system. 
   
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
       
## Issues
---

1. The collision between  sphere was not entirely matching up (will delay) with the cursor change.
2. There was no self-collision applied for the cloth particles.
3. The particles were not surfaced out and rendered as a polygon.
4. The simulation might produce a floaty result or explode due to coefficients and dynamic factors.

---




