#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#endif

#include <map>
#include <bufferevent.h>
#include <buffer.h>
#include <listener.h>
#include <util.h>
#include <event.h>
#include "protocol_dev.h"
#include "client.h"
#include "http.h"
#include "keyvalq_struct.h"
#include "util.h"
#include "http_compat.h"
#include "http_struct.h"
#include "http_server.h"
#include "socket_event.h"
#include "msg_deal_queue.h"
#include "protocol_dev.h"
#include "cJSON.h"
#include "mysql_class.h"
#include "threadpool.h"
#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"

//#ifdef __cplusplus
//extern "C" {
//#endif
static void			*http_server_thread(void *args);
static const char 	*guess_content_type(const char *path);
static void     	send_document_cb(struct evhttp_request *req, void *arg);
static void 		dump_request_cb(struct evhttp_request *req, void *arg);

static uint32 	run_time=0;//s




void  			httpd_handler();

char uri_root[512];
int  index_html_len = 0;
char index_html_buf[10*1024];

static const struct table_entry
{
    const char *extension;
    const char *content_type;
}
content_type_table[] = {
                           { "txt", "text/plain" },
                           { "c", "text/plain" },
                           { "h", "text/plain" },
                           { "html", "text/html" },
                           { "htm", "text/htm" },
                           { "css", "text/css" },
                           { "gif", "image/gif" },
                           { "jpg", "image/jpeg" },
                           { "jpeg", "image/jpeg" },
                           { "png", "image/png" },
                           { "pdf", "application/pdf" },
                           { "ps", "application/postsript" },
                           { NULL, NULL },
                       };

pthread_t    	http_thread_id;

struct 			event_base *base_HTTP;
struct 			evhttp *http;
struct 			evhttp_bound_socket *handle;
unsigned short 	port = 1212;

/* Try to guess a good content-type for 'path' */
static const char *guess_content_type(const char *path)
{
    const char *last_period, *extension;
    const struct table_entry *ent;
    last_period = strrchr(path, '.');
    if (!last_period || strchr(last_period, '/'))
        goto not_found; /* no exension */
    extension = last_period + 1;
    for (ent = &content_type_table[0]; ent->extension; ++ent)
    {
        if (!evutil_ascii_strcasecmp(ent->extension, extension))
            return ent->content_type;
    }

not_found:
    return "application/misc";
}



int   http_server_init(int  listen_port)
{

    run_time = time(0);

    int iRet;
    port = listen_port;
    iRet = pthread_create(&http_thread_id, NULL, http_server_thread, &listen_port);
    if (iRet == -1)
    {
        printf("can't creat http_server_thread thread\n");
    }
    return 0;
}


//http  work thread
void* http_server_thread(void *args)
{
    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;
    char			uri_root[512];


#ifdef WIN32

    WSADATA WSAData;
    WSAStartup(0x101, &WSAData);
#else

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return NULL;
#endif

    cout<<"http sever start port="<<port<<"\n";

    base = event_base_new();
    if (!base)
    {
        fprintf(stderr, "Couldn't create an event_base: exiting\n");
        return NULL;
    }

    /* Create a new evhttp object to handle requests. */
    http = evhttp_new(base);
    if (!http)
    {
        fprintf(stderr, "couldn't create evhttp. Exiting.\n");
        return NULL;
    }

    /* The /dump URI will dump all requests to stdout and say 200 ok. */
    evhttp_set_cb(http, "/dump", dump_request_cb, NULL);

    /* We want to accept arbitrary requests, so we need to set a "generic"
     * cb.	We can also add callbacks for specific paths. */
    //	evhttp_set_gencb(http, send_document_cb, argv[1]);
    evhttp_set_gencb(http, send_document_cb, NULL);

    /* Now we tell the evhttp what port to listen on */
    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);
    if (!handle)
    {
        fprintf(stderr, "couldn't bind to port %d. Exiting.\n",
                (int)port);
        return NULL;
    }

    {
        /* Extract and display the address we're listening on. */
        struct sockaddr_storage ss;
        evutil_socket_t fd;
        ev_socklen_t socklen = sizeof(ss);
        char addrbuf[128];
        void *inaddr;
        const char *addr;
        int got_port = -1;
        fd = evhttp_bound_socket_get_fd(handle);
        memset(&ss, 0, sizeof(ss));
        if (getsockname(fd, (struct sockaddr *)&ss, &socklen))
        {
            perror("getsockname() failed");
            return NULL;
        }
        if (ss.ss_family == AF_INET)
        {
            got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
            inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
        }
        else if (ss.ss_family == AF_INET6)
        {
            got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
            inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
        }
        else
        {
            fprintf(stderr, "Weird address family %d\n",
                    ss.ss_family);
            return NULL;
        }
        addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf,
                                sizeof(addrbuf));
        if (addr)
        {
            printf("Listening on %s:%d\n", addr, got_port);
            evutil_snprintf(uri_root, sizeof(uri_root),
                            "http://%s:%d",addr,got_port);
        }
        else
        {
            fprintf(stderr, "evutil_inet_ntop failed\n");
            return NULL;
        }
    }

    event_base_dispatch(base);

}

/* Callback used for the /dump URI, and for every non-GET request:
 * dumps all information to stdout and gives back a trivial 200 ok */
static void dump_request_cb(struct evhttp_request *req, void *arg)
{
    const char *cmdtype;
    struct evkeyvalq *headers;
    struct evkeyval *header;
    struct evbuffer *buf;

    switch (evhttp_request_get_command(req))
    {
    case EVHTTP_REQ_GET:
        cmdtype = "GET";
        break;
    case EVHTTP_REQ_POST:
        cmdtype = "POST";
        break;
    case EVHTTP_REQ_HEAD:
        cmdtype = "HEAD";
        break;
    case EVHTTP_REQ_PUT:
        cmdtype = "PUT";
        break;
    case EVHTTP_REQ_DELETE:
        cmdtype = "DELETE";
        break;
    case EVHTTP_REQ_OPTIONS:
        cmdtype = "OPTIONS";
        break;
    case EVHTTP_REQ_TRACE:
        cmdtype = "TRACE";
        break;
    case EVHTTP_REQ_CONNECT:
        cmdtype = "CONNECT";
        break;
    case EVHTTP_REQ_PATCH:
        cmdtype = "PATCH";
        break;
    default:
        cmdtype = "unknown";
        break;
    }

    printf("Received a %s request for %s\nHeaders:\n",
           cmdtype, evhttp_request_get_uri(req));

    headers = evhttp_request_get_input_headers(req);
    for (header = headers->tqh_first; header;
            header = header->next.tqe_next)
    {
        printf("  %s: %s\n", header->key, header->value);
    }

    buf = evhttp_request_get_input_buffer(req);
    puts("Input data: <<<");
    while (evbuffer_get_length(buf))
    {
        int n;
        char cbuf[128];
        n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
        if (n > 0)
            (void) fwrite(cbuf, 1, n, stdout);
    }
    puts(">>>");

    evhttp_send_reply(req, 200, "OK", NULL);
}
/*
	*********************************************************
*/
string   Creat_Run_Status_Json(void)
{
    string   tmpjsonstr;
    cJSON* 		 pRoot;

    pRoot = cJSON_CreateObject();

   // std::unique_lock<std::mutex>   lck(Map_Dev_online.lock);

    Map_Dev_online.Map.size();
	cJSON_AddNumberToObject(pRoot, "conn num",Map_Data_Dev.Map.size());
    cJSON_AddNumberToObject(pRoot, "device num",Map_Dev_online.Map.size());
    cJSON_AddNumberToObject(pRoot, "running",time(0) - run_time);

    tmpjsonstr = cJSON_Print(pRoot);

    return tmpjsonstr;
}
/*
	*********************************************************
*/

int  send_data_to_dev(struct evbuffer *evb,struct evhttp_request *req,string dev_id, string ctrl_value)
{
	std::unique_lock<std::mutex> lck (Map_Dev_online.lock);
	Map_Dev_online_iter  m1_iter;
	m1_iter = Map_Dev_online.Map.find(dev_id);
	if(m1_iter != Map_Dev_online.Map.end())
	{
		boost::shared_ptr<device_client> tempclient = m1_iter->second;
		
		string tmp_msg = "{\"ver\":\"1.0\",\"cmdtype\":\"1\",\"data\":\""+ctrl_value+"\"}";
		bev_data_struct data_t;

		data_t.bev = tempclient->bev;
	    data_t.data_len =tmp_msg.length();
		memset(data_t.data,0,sizeof(data_t.data));
		tmp_msg.copy(data_t.data,data_t.data_len,0);

		cout<<"tmp_msg->"<<tmp_msg<<endl;
		
    	Device_Send_queue.Data_Queue_Push(&data_t);
		
		
		evbuffer_add_printf(evb, "send ok!");
		evhttp_send_reply(req, 200, "OK", evb);
		return 0;
	}
	else
	{
		evbuffer_add_printf(evb, "no find device!");
		evhttp_send_reply(req, 200, "OK", evb);
		return 1;
	}


}

/* This callback gets invoked when we get any http request that doesn't match
 * any other callback.  Like any evhttp server callback, it has a simple job:
 * it must eventually call evhttp_send_error() or evhttp_send_reply().
 */
static void send_document_cb(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = NULL;
    const char *docroot = (const char *)arg;
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = NULL;
    const char *path;
    char *decoded_path;
    char *whole_path = NULL;
    size_t len;
    int fd = -1;
    struct stat st;

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET)
    {
        dump_request_cb(req, arg);
        return;
    }
    printf("GET request<%s>\n",	uri);
    //-----------------------
    evb = evbuffer_new();

    if ( uri != NULL  )
    {
        //run status
        if (strstr(uri, "api.ctrl"))
        {
            if(strstr(uri, "?dev_id="))//
            {
           		struct evkeyvalq params;
                evhttp_parse_query(uri, &params);
                char *device_id_tmp = (char *)evhttp_find_header(&params, "dev_id");
                
				if(strstr(uri, "ctrl="))
				{
					char *ctrl_value = (char *)evhttp_find_header(&params, "ctrl");
					if(device_id_tmp != NULL && ctrl_value!= NULL)
					{
						send_data_to_dev(evb,req, device_id_tmp, ctrl_value);								
					}
				}
                else if ( device_id_tmp != NULL)
                {
                    std::unique_lock<std::mutex> lck (Map_Dev_online.lock);
                    Map_Dev_online_iter  m1_iter;
                    m1_iter = Map_Dev_online.Map.find(device_id_tmp);
                    if(m1_iter != Map_Dev_online.Map.end())
                    {
                        boost::shared_ptr<device_client>  tempclient = m1_iter->second;
                        string  jsonstr;
                        jsonstr = tempclient->get_json_info();
                        if (jsonstr.c_str() == NULL)
                        {
                            jsonstr = "device timeout\n";
                            Map_Dev_online.Map.erase(m1_iter);
							//------------------------------------------------------------------
                            {
                                std::unique_lock<std::mutex> lck (Map_Data_Dev.lock);
                                Map_Parse_Dev_iter  m1_iter;
                                m1_iter = Map_Data_Dev.Map.find(tempclient->bev);
                                if ( m1_iter != Map_Data_Dev.Map.end() )
                                {
                                    bufferevent_free(tempclient->bev);
                                    Map_Data_Dev.Map.erase(m1_iter);
                                }
                            }
                        }
                        evbuffer_add_printf(evb, "%s", (const char*)(jsonstr.c_str()));
                        evhttp_send_reply(req, 200, "OK", evb);
                    }
                    else
                    {
                        evbuffer_add_printf(evb, "%s", "no find device!");
                        evhttp_send_reply(req, 200, "OK", evb);
                    }
                }
                else
                {
                    evbuffer_add_printf(evb, "%s", "get  device id !");
                    evhttp_send_reply(req, 200, "OK", evb);
                }

            }
            else
            {
                evbuffer_add_printf(evb, "%s",(const char*)(Creat_Run_Status_Json().c_str()));
                evhttp_send_reply(req, 200, "OK", evb);
            }
        }
        else
        {
//            goto ERROR_URL;
        }
    }
    else
    {
//ERROR_URL:
        //	char   htmlchar[1024*10];
        //	int  len;
//        	static int reload_count = 0;
//        cout<<"reload_cout"<<reload_count<<endl;
//	if(index_html_len == 0 || reload_count <= 0)
//        {
//            
//	    cout<<"reload index.html\n";
//		FILE  *fp;
//            fp = fopen("./index.html","r");
//            if (fp != NULL)
//            {
//                index_html_len= fread(index_html_buf, 1, sizeof(index_html_buf),fp);
//                fclose(fp);
//                evbuffer_add(evb,index_html_buf, index_html_len);
//                evhttp_send_reply(req, 200, "OK", evb);
//		reload_count = 5;
//            }
//            else
//            {
//                evbuffer_add_printf(evb,"no find index.html");
//            }
//        }
//        else
//        {
//	    reload_count--;
//            evbuffer_add(evb,index_html_buf, index_html_len);
//            evhttp_send_reply(req, 200, "OK", evb);
//        }

    }

    if (evb)
    {
        evbuffer_free(evb);
    }
    return ;

    /* Decode the URI */
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        printf("It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    /* Let's see what path the user asked for. */
    path = evhttp_uri_get_path(decoded);
    if (!path)
        path = "/";


    /* We need to decode it, to see what path the user really wanted. */
    decoded_path = evhttp_uridecode(path, 0, NULL);

    if (decoded_path == NULL)
        goto err;
    /* Don't allow any ".."s in the path, to avoid exposing stuff outside
     * of the docroot.  This test is both overzealous and underzealous:
     * it forbids aceptable paths like "/this/one..here", but it doesn't
     * do anything to prevent symlink following." */

    if (strstr(decoded_path, ".."))
        goto err;

    int  docroot_len ;
    if ( docroot ==NULL )
    {
        docroot_len = 0;
    }
    else
    {
        docroot_len = strlen(docroot);
    }

    len = strlen(decoded_path)+docroot_len+2;
    if (!(whole_path = (char*)malloc(len)))
    {
        perror("malloc");
        goto err;
    }

    evutil_snprintf(whole_path, len, "%s/%s", docroot, decoded_path);

    if (stat(whole_path, &st)<0)
    {
        goto err;
    }


    /* This holds the content we're sending. */
    evb = evbuffer_new();

    if (S_ISDIR(st.st_mode))
    {
        /* If it's a directory, read the comments and make a little
         * index page */
#ifdef WIN32
        HANDLE d;
        WIN32_FIND_DATAA ent;
        char *pattern;
        size_t dirlen;
#else

        DIR *d;
        struct dirent *ent;
#endif

        const char *trailing_slash = "";

        if (!strlen(path) || path[strlen(path)-1] != '/')
            trailing_slash = "/";

#ifdef WIN32

        dirlen = strlen(whole_path);
        pattern = malloc(dirlen+3);
        memcpy(pattern, whole_path, dirlen);
        pattern[dirlen] = '\\';
        pattern[dirlen+1] = '*';
        pattern[dirlen+2] = '\0';
        d = FindFirstFileA(pattern, &ent);
        free(pattern);
        if (d == INVALID_HANDLE_VALUE)
            goto err;
#else

        if (!(d = opendir(whole_path)))
            goto err;
#endif

        evbuffer_add_printf(evb, "<html>\n <head>\n"
                            "  <title>%s</title>\n"
                            "  <base href='%s%s%s'>\n"
                            " </head>\n"
                            " <body>\n"
                            "  <h1>%s</h1>\n"
                            "  <ul>\n",
                            decoded_path, /* XXX html-escape this. */
                            uri_root, path, /* XXX html-escape this? */
                            trailing_slash,
                            decoded_path /* XXX html-escape this */);
#ifdef WIN32

        do
        {
            const char *name = ent.cFileName;
#else

        while ((ent = readdir(d)))
        {
            const char *name = ent->d_name;
#endif

            evbuffer_add_printf(evb,
                                "    <li><a href=\"%s\">%s</a>\n",
                                name, name);/* XXX escape this */
#ifdef WIN32

        }
        while (FindNextFileA(d, &ent))
            ;
#else

        }
#endif
        evbuffer_add_printf(evb, "</ul></body></html>\n");
#ifdef WIN32

        FindClose(d);
#else

        closedir(d);
#endif

        evhttp_add_header(evhttp_request_get_output_headers(req),
                          "Content-Type", "text/html");
    }
    else
    {
        /* Otherwise it's a file; add it to the buffer to get
         * sent via sendfile */
        const char *type = guess_content_type(decoded_path);
        if ((fd = open(whole_path, O_RDONLY)) < 0)
        {
            perror("open");
            goto err;
        }

        if (fstat(fd, &st)<0)
        {
            /* Make sure the length still matches, now that we
             * opened the file :/ */
            perror("fstat");
            goto err;
        }
        evhttp_add_header(evhttp_request_get_output_headers(req),
                          "Content-Type", type);
        evbuffer_add_file(evb, fd, 0, st.st_size);
    }

    evhttp_send_reply(req, 200, "OK", evb);
    goto done;
err:
    evhttp_send_error(req, 404, "Document was not found");
    if (fd>=0)
        close(fd);
done:
    if (decoded)
        evhttp_uri_free(decoded);
    if (decoded_path)
        free(decoded_path);
    if (whole_path)
        free(whole_path);
    if (evb)
        evbuffer_free(evb);
}

static void syntax(void)
{
    fprintf(stdout, "Syntax: http-server <docroot>\n");
}



//#ifdef __cplusplus
//};
//#endif

