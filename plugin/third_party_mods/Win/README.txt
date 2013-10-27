May 17, 2013

Notes on Third Party Mods
-------------------------------
When the FireBreath project is compiled, the C++ linker fails because DllGetClassObject and DllCanUnloadNow 
are defined in FireBreath and also in DirectShow baseclasses. 
(libjingle\trunk\third_party\winsdk_samples\src\Samples\multimedia\directshow\baseclasses).

Unfortunately, we need to use the DllGetClassObject and DllCanUnloadNow classes defined in FireBreath.

To resolve the Duplicate Classes issues, for now I renamed DllGetClassObject and DllCanUnloadNow classes in
libjingle\trunk\third_party\winsdk_samples\src\Samples\multimedia\directshow\baseclasses\dllentry.cpp

At this time, we simply overide the standard dllentry.cpp with a modified version of the same file in
plugin\third_party_mods\Win directory.

