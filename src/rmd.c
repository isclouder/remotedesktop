/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "rmd_start.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <libudev.h>
#include <X11/extensions/Xrandr.h>
 
#undef asmlinkage
#ifdef __i386__
#define asmlinkage __attribute__((regparm(0)))
#else
#define asmlinkage 
#endif
 
#define MAX_SCREENS 16
 
static int udev_exit;
char *display_name = ":0";
int screen_qxl = 1;
pid_t fpid;

void x11_xrandr(Display *dpy, int screen, int width, int height){
    Window   root;
    XRRScreenConfiguration *sc;
    XRRScreenSize *sizes;
    int         nsize;
    int         size = -1;
    Status      status = RRSetConfigFailed;

    printf("set screen(%d) to %dx%d\n", screen,width,height);

    root = RootWindow (dpy, screen);
    sc = XRRGetScreenInfo (dpy, root);
    if (sc == NULL)
        exit (1);

    sizes = XRRConfigSizes(sc, &nsize);
    for (size = 0; size < nsize; size++){
        if (sizes[size].width == width && sizes[size].height == height)
            break;
    }
    if (size >= nsize){
        fprintf (stderr,
                 "Size %dx%d not found in available modes\n", width, height);
        exit (1);
    }
    status = XRRSetScreenConfigAndRate (dpy, sc, root, (SizeID) size, (Rotation) (1),
                                            0.000000, 0);
    if (status == RRSetConfigFailed){
        printf ("Failed to change the screen configuration!\n");
    } 
}

static void x11_xrandr_1(Display *dpy, int screen, XRRScreenResources *res){
    int i;
    for (i = 0 ; i <res->nmode ; ++i){
        printf("screen(%d):%dx%d\n",screen,res->modes[i].width,res->modes[i].height);
    }
    x11_xrandr(dpy, screen, res->modes[0].width, res->modes[0].height);
}

static void x11_xrandr_0(Display *dpy, int screen, XRRScreenResources *res, int width, int height){
    int i=0, m=0;
    int w = DisplayWidth(dpy, screen);
    int h = DisplayHeight(dpy, screen);
    for (i = 0 ; i <res->nmode ; ++i){
        printf("screen(%d):%dx%d\n",screen,res->modes[i].width,res->modes[i].height);
    }
    if(w==width && h==height){
        return;
    }

    for (i = 0 ; i <res->nmode ; ++i){
	if (res->modes[i].width<=width && res->modes[i].height<=height){
            m = i;
            break;
	}
    }
    for (i = 0 ; i <res->nmode ; ++i){
	if (res->modes[i].width==width && res->modes[i].height==height){
            m = i;
            break;
	}
    }

    if(w==res->modes[m].width && h==res->modes[m].height){
        return;
    }
    x11_xrandr(dpy, screen, res->modes[m].width, res->modes[m].height);
 
    kill(fpid, SIGTERM); 
    pid_t pid;
    pid=fork();
    if(pid < 0){
	printf("fork error\n");
    }
    else if(pid == 0){
	rmd_start();
    }
    else{
	printf("rmd new pid is:%d\n", pid);
	fpid = pid;
    }
}

static void x11_get_mode(){
    int i;
    Display *display;
    int screen_count;
    Window root_window[MAX_SCREENS];
    XRRScreenResources *res0;
    XRRScreenResources *res1;

    display = XOpenDisplay(display_name);
    if (!display){
        printf("could not connect to X-server\n");
        return;
    }

    screen_count = ScreenCount(display);
    if (screen_count > MAX_SCREENS){
        printf("Error too much screens: %d > %d\n",
               screen_count, MAX_SCREENS);
        XCloseDisplay(display);
        return;
    }
    for (i = 0; i < screen_count; i++)
        root_window[i] = RootWindow(display, i);

    res1 = XRRGetScreenResources(display, root_window[screen_qxl]);
    x11_xrandr_1(display,screen_qxl,res1);

    res0 = XRRGetScreenResources(display, root_window[0]);
    x11_xrandr_0(display,0,res0, res1->modes[0].width, res1->modes[0].height);

    XRRFreeScreenResources(res0);
    XRRFreeScreenResources(res1);
    XCloseDisplay(display);
}

static void asmlinkage sig_handler(int signum){
	if (signum == SIGINT || signum == SIGTERM)
		udev_exit = 1;
}
static void sig_child(int signum){
    pid_t pid;
    int stat;
    pid = wait(&stat);    
    printf( "child %d exit\n", pid);
    return;
} 
 
static void print_device(struct udev_device *device, const char *source, int env){
	if (env){
		struct udev_list_entry *list_entry;
		udev_list_entry_foreach(list_entry, udev_device_get_properties_list_entry(device))
			printf("%s=%s\n",
			       udev_list_entry_get_name(list_entry),
			       udev_list_entry_get_value(list_entry));
		printf("\n");
	}

        if (strcmp(udev_device_get_subsystem(device),"drm") == 0){
            FILE *fp;
            char path[256], vendor[7];
            memset(path,0,256);
            memset(vendor,0,7);
            strcpy(path, udev_device_get_syspath(device)); 
            strcat(path, "/device/vendor"); 
            fp=fopen(path, "r");
            fgets(vendor, 7, fp);
            if (strcmp(vendor,"0x1b36") == 0){
                x11_get_mode();
            }
            fclose(fp);
        }
 
}
 
 
int udevadm_monitor(struct udev *udev){
	struct sigaction act;
	int env = 0;
	int print_kernel = 1;
	struct udev_monitor *kernel_monitor = NULL;
	fd_set readfds;
	int rc = 0;
 
	if (getuid() != 0){
		fprintf(stderr, "root privileges needed to subscribe to kernel events\n");
		goto out;
	}
 
	/* set signal handlers */
	memset(&act, 0x00, sizeof(struct sigaction));
	act.sa_handler = (void (*)(int)) sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	signal(SIGCHLD,  &sig_child);
 
	if (print_kernel){
		kernel_monitor = udev_monitor_new_from_netlink(udev, "udev");
		if (kernel_monitor == NULL){
			rc = 3;
			printf("udev_monitor_new_from_netlink() error\n");
			goto out;
		}
 
		if (udev_monitor_enable_receiving(kernel_monitor) < 0){
			rc = 4;
			goto out;
		}
 
		//printf("UEVENT the kernel uevent: \n");
	}
	
	printf("\n");
        x11_get_mode();
	while (!udev_exit){
		int fdcount;
		FD_ZERO(&readfds);
 
		if (kernel_monitor != NULL)
			FD_SET(udev_monitor_get_fd(kernel_monitor), &readfds);
 
		fdcount = select(udev_monitor_get_fd(kernel_monitor)+1, &readfds, NULL, NULL, NULL);
		if (fdcount < 0){
			if (errno != EINTR)
				fprintf(stderr, "error receiving uevent message: %m\n");
			continue;
		}
 
		if ((kernel_monitor != NULL) && FD_ISSET(udev_monitor_get_fd(kernel_monitor), &readfds)){
			struct udev_device *device;
			device = udev_monitor_receive_device(kernel_monitor);
			if (device == NULL)
				continue;
			print_device(device, "UEVENT", env);
			udev_device_unref(device);
		}
 
	}
 
out:
	udev_monitor_unref(kernel_monitor);
	return rc;
}


int main(int argc, char *argv[]){
    struct udev *udev;
    pid_t pid;

    pid=fork();
    if(pid < 0){
	printf("fork error\n");
	return -1;
    }
    else if(pid == 0){
	rmd_start();
    }
    else{
	printf("main pid is:%d\n", getpid());
	printf("rmd pid is:%d\n", pid);
        fpid = pid;
        udev = udev_new();
        if (udev == NULL){
            udev_unref(udev);
            return -1;
        }
        udevadm_monitor(udev);
        udev_unref(udev);
    }
    printf("main pid exit\n");
    return 0;
}

