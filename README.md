# Windows 32-bit more than 4 GB example

This repo contains an example of how a 32-bit Windows application can use more than 4 GB of RAM by mapping and unmapping memory on-demand. 

When executed, the application uses [CreateFileMappingA](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createfilemappinga) with a NULL file to allocate 8 GB of RAM without mapping it. It then uses [MapViewOfFile](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-mapviewoffile) to map 1 GB of that allocated memory, fill it up with data, then use [UnmapViewOfFile](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile) to unmap it. This is repeated until all 8 GBs are filled up with data.

After this, the program remains open until a key is pressed. You can use that opportunity to check the task manager: the memory usage graph should show 8 GB of RAM as "committed" but not "in use". This means the memory is allocated but not mapped. 
