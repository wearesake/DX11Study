#pragma once

#define CHWND_EXCEPT( hr ) MyWindow::HrException( __LINE__,__FILE__,(hr) )
#define CHWND_LAST_EXCEPT() MyWindow::HrException( __LINE__,__FILE__, GetLastError() )
#define CHWND_NOGFX_EXCEPT() MyWindow::NoGuardException( __LINE__,__FILE__ )
