## UE5.1_Plugin_TestTask

Task was done in under 6h. 

With few screenshots i'll try to showcase how to use it ( or how it does what it's supposed to)

Place a VolumeSpawnerActor on the scene.
![UnrealEditor_EF4T3uSaCH](https://github.com/nescave/UE5.1_Plugin_TestTask/assets/38581117/07c3b8b0-babe-4464-bbde-b550ffe6c1a6)

Marked red are important things to controll about the actor details
User can specify size of the box in which actors will spawn (Transform of actor also works).
User have to choose mesh to start spawning and specify number of meshes to spawn
Initial name isn't required. With it User can override name of the asset which is the default option.
![UnrealEditor_OmZXX9IPR2](https://github.com/nescave/UE5.1_Plugin_TestTask/assets/38581117/643bf4f0-972a-480d-9e8f-64ffafe71d5b)

After filling up the needed data you can hit SpawnMeshes.
![UnrealEditor_vlQO0WaPwa](https://github.com/nescave/UE5.1_Plugin_TestTask/assets/38581117/7c5800bd-fbbd-4871-94c9-14d9b81201f3)

Colors are made using unlit, emissive materials. Each mesh has it's own DynamicInstance of a given material.

Last is the DataTable:
![UnrealEditor_9S3MX5Zqem](https://github.com/nescave/UE5.1_Plugin_TestTask/assets/38581117/fa3663bb-2617-49b1-ab64-433c9f3e4569)
Each time user respawns the mesh actors, new DataTable is constructed. If old DataTable has the same name as the new one,
unreal will ask if you wish to replace it or abort the process. 
