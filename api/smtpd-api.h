/*	$OpenBSD$	*/

/*
 * Copyright (c) 2013 Eric Faurot <eric@openbsd.org>
 * Copyright (c) 2011 Gilles Chehade <gilles@poolp.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef	_SMTPD_API_H_
#define	_SMTPD_API_H_

#include <sys/queue.h>
#include <sys/tree.h>
#include <sys/socket.h>

#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>

#if 0
#include <event.h>
#endif
#include <imsg.h>

#include "smtpd-defines.h"
#if 0
#include "ioev.h"
#include "iobuf.h"
#include "log.h"
#include "rfc2822.h"
#endif

#define	FILTER_API_VERSION	 52

enum blockmodes {
	BM_NORMAL,
	BM_NONBLOCK
};

struct mproc {
	pid_t		 pid;
	char		*name;
	int		 proc;
	void		(*handler)(struct mproc *, struct imsg *);
	struct imsgbuf	 imsgbuf;

	char		*m_buf;
	size_t		 m_alloc;
	size_t		 m_pos;
	uint32_t	 m_type;
	uint32_t	 m_peerid;
	pid_t		 m_pid;
	int		 m_fd;

	int		 enable;
#if 0
	short		 events;
	struct event	 ev;
#endif
	void		*data;

	off_t		 msg_in;
	off_t		 msg_out;
	off_t		 bytes_in;
	off_t		 bytes_out;
	size_t		 bytes_queued;
	size_t		 bytes_queued_max;
};

struct msg {
	const uint8_t	*pos;
	const uint8_t	*end;
};

struct mailaddr {
	char	user[SMTPD_MAXLOCALPARTSIZE];
	char	domain[SMTPD_MAXDOMAINPARTSIZE];
};

SPLAY_HEAD(_dict, dictentry);
SPLAY_HEAD(_tree, treeentry);

struct tree {
	struct _tree	tree;
	size_t		count;
};

struct dict {
	struct _dict	dict;
	size_t		count;
};

enum filter_status {
	FILTER_OK,
	FILTER_FAIL,
	FILTER_CLOSE,
};

enum filter_imsg {
	IMSG_FILTER_REGISTER,
	IMSG_FILTER_EVENT,
	IMSG_FILTER_QUERY,
	IMSG_FILTER_PIPE,
	IMSG_FILTER_RESPONSE
};

/* XXX - server side requires mfa_session.c update on filter_event */
enum filter_event_type {
	EVENT_CONNECT,
	EVENT_RESET,
	EVENT_DISCONNECT,
	EVENT_TX_BEGIN,
	EVENT_TX_COMMIT,
	EVENT_TX_ROLLBACK,
};

/* XXX - server side requires mfa_session.c update on filter_hook changes */
enum filter_query_type {
	QUERY_CONNECT,
	QUERY_HELO,
	QUERY_MAIL,
	QUERY_RCPT,
	QUERY_DATA,
	QUERY_EOM,
	QUERY_DATALINE,
};

struct filter_connect {
	struct sockaddr_storage	 local;
	struct sockaddr_storage	 remote;
	const char		*hostname;
};

#define PROC_QUEUE_API_VERSION	2

enum {
	PROC_QUEUE_OK,
	PROC_QUEUE_FAIL,
	PROC_QUEUE_INIT,
	PROC_QUEUE_CLOSE,
	PROC_QUEUE_MESSAGE_CREATE,
	PROC_QUEUE_MESSAGE_DELETE,
	PROC_QUEUE_MESSAGE_COMMIT,
	PROC_QUEUE_MESSAGE_FD_R,
	PROC_QUEUE_MESSAGE_CORRUPT,
	PROC_QUEUE_MESSAGE_UNCORRUPT,
	PROC_QUEUE_ENVELOPE_CREATE,
	PROC_QUEUE_ENVELOPE_DELETE,
	PROC_QUEUE_ENVELOPE_LOAD,
	PROC_QUEUE_ENVELOPE_UPDATE,
	PROC_QUEUE_ENVELOPE_WALK,
	PROC_QUEUE_MESSAGE_WALK,
};

#define PROC_SCHEDULER_API_VERSION	2

struct scheduler_info;

enum {
	PROC_SCHEDULER_OK,
	PROC_SCHEDULER_FAIL,
	PROC_SCHEDULER_INIT,
	PROC_SCHEDULER_INSERT,
	PROC_SCHEDULER_COMMIT,
	PROC_SCHEDULER_ROLLBACK,
	PROC_SCHEDULER_UPDATE,
	PROC_SCHEDULER_DELETE,
	PROC_SCHEDULER_HOLD,
	PROC_SCHEDULER_RELEASE,
	PROC_SCHEDULER_BATCH,
	PROC_SCHEDULER_MESSAGES,
	PROC_SCHEDULER_ENVELOPES,
	PROC_SCHEDULER_SCHEDULE,
	PROC_SCHEDULER_REMOVE,
	PROC_SCHEDULER_SUSPEND,
	PROC_SCHEDULER_RESUME,
};

enum envelope_flags {
	EF_AUTHENTICATED	= 0x01,
	EF_BOUNCE		= 0x02,
	EF_INTERNAL		= 0x04, /* Internal expansion forward */

	/* runstate, not saved on disk */

	EF_PENDING		= 0x10,
	EF_INFLIGHT		= 0x20,
	EF_SUSPEND		= 0x40,
	EF_HOLD			= 0x80,
};

struct evpstate {
	uint64_t		evpid;
	uint16_t		flags;
	uint16_t		retry;
	time_t			time;
};

enum delivery_type {
	D_MDA,
	D_MTA,
	D_BOUNCE,
};

struct scheduler_info {
	uint64_t		evpid;
	enum delivery_type	type;
	uint16_t		retry;
	time_t			creation;
	time_t			expire;
	time_t			lasttry;
	time_t			lastbounce;
	time_t			nexttry;
};

#define SCHED_REMOVE		0x01
#define SCHED_EXPIRE		0x02
#define SCHED_UPDATE		0x04
#define SCHED_BOUNCE		0x08
#define SCHED_MDA		0x10
#define SCHED_MTA		0x20

#define PROC_TABLE_API_VERSION	2

struct table_open_params {
	uint32_t	version;
	char		name[LINE_MAX];
};

enum table_service {
	K_NONE		= 0x000,
	K_ALIAS		= 0x001,	/* returns struct expand	*/
	K_DOMAIN	= 0x002,	/* returns struct destination	*/
	K_CREDENTIALS	= 0x004,	/* returns struct credentials	*/
	K_NETADDR	= 0x008,	/* returns struct netaddr	*/
	K_USERINFO	= 0x010,	/* returns struct userinfo	*/
	K_SOURCE	= 0x020,	/* returns struct source	*/
	K_MAILADDR	= 0x040,	/* returns struct mailaddr	*/
	K_ADDRNAME	= 0x080,	/* returns struct addrname	*/
	K_MAILADDRMAP	= 0x100,	/* returns struct mailaddr	*/
};
#define K_ANY		  0xfff

enum {
	PROC_TABLE_OK,
	PROC_TABLE_FAIL,
	PROC_TABLE_OPEN,
	PROC_TABLE_CLOSE,
	PROC_TABLE_UPDATE,
	PROC_TABLE_CHECK,
	PROC_TABLE_LOOKUP,
	PROC_TABLE_FETCH,
};

enum enhanced_status_code {
	/* 0.0 */
	ESC_OTHER_STATUS				= 00,

	/* 1.x */
	ESC_OTHER_ADDRESS_STATUS			= 10,
	ESC_BAD_DESTINATION_MAILBOX_ADDRESS		= 11,
	ESC_BAD_DESTINATION_SYSTEM_ADDRESS		= 12,
	ESC_BAD_DESTINATION_MAILBOX_ADDRESS_SYNTAX     	= 13,
	ESC_DESTINATION_MAILBOX_ADDRESS_AMBIGUOUS	= 14,
	ESC_DESTINATION_ADDRESS_VALID			= 15,
	ESC_DESTINATION_MAILBOX_HAS_MOVED      		= 16,
	ESC_BAD_SENDER_MAILBOX_ADDRESS_SYNTAX		= 17,
	ESC_BAD_SENDER_SYSTEM_ADDRESS			= 18,

	/* 2.x */
	ESC_OTHER_MAILBOX_STATUS			= 20,
	ESC_MAILBOX_DISABLED				= 21,
	ESC_MAILBOX_FULL				= 22,
	ESC_MESSAGE_LENGTH_TOO_LARGE   			= 23,
	ESC_MAILING_LIST_EXPANSION_PROBLEM		= 24,

	/* 3.x */
	ESC_OTHER_MAIL_SYSTEM_STATUS			= 30,
	ESC_MAIL_SYSTEM_FULL				= 31,
	ESC_SYSTEM_NOT_ACCEPTING_MESSAGES		= 32,
	ESC_SYSTEM_NOT_CAPABLE_OF_SELECTED_FEATURES    	= 33,
	ESC_MESSAGE_TOO_BIG_FOR_SYSTEM		    	= 34,
	ESC_SYSTEM_INCORRECTLY_CONFIGURED      	    	= 35,

	/* 4.x */
	ESC_OTHER_NETWORK_ROUTING_STATUS      	    	= 40,
	ESC_NO_ANSWER_FROM_HOST		      	    	= 41,
	ESC_BAD_CONNECTION		      	    	= 42,
	ESC_DIRECTORY_SERVER_FAILURE   	      	    	= 43,
	ESC_UNABLE_TO_ROUTE	   	      	    	= 44,
	ESC_MAIL_SYSTEM_CONGESTION   	      	    	= 45,
	ESC_ROUTING_LOOP_DETECTED   	      	    	= 46,
	ESC_DELIVERY_TIME_EXPIRED   	      	    	= 47,

	/* 5.x */
	ESC_INVALID_RECIPIENT				= 50,
	ESC_INVALID_COMMAND	   	      	    	= 51,
	ESC_SYNTAX_ERROR	   	      	    	= 52,
	ESC_TOO_MANY_RECIPIENTS	   	      	    	= 53,
	ESC_INVALID_COMMAND_ARGUMENTS  	      	    	= 54,
	ESC_WRONG_PROTOCOL_VERSION  	      	    	= 55,

	/* 6.x */
	ESC_OTHER_MEDIA_ERROR   	      	    	= 60,
	ESC_MEDIA_NOT_SUPPORTED   	      	    	= 61,
	ESC_CONVERSION_REQUIRED_AND_PROHIBITED		= 62,
	ESC_CONVERSION_REQUIRED_BUT_NOT_SUPPORTED      	= 63,
	ESC_CONVERSION_WITH_LOSS_PERFORMED	     	= 64,
	ESC_CONVERSION_FAILED			     	= 65,

	/* 7.x */
	ESC_OTHER_SECURITY_STATUS      		     	= 70,
	ESC_DELIVERY_NOT_AUTHORIZED_MESSAGE_REFUSED	= 71,
	ESC_MAILING_LIST_EXPANSION_PROHIBITED		= 72,
	ESC_SECURITY_CONVERSION_REQUIRED_NOT_POSSIBLE  	= 73,
	ESC_SECURITY_FEATURES_NOT_SUPPORTED	  	= 74,
	ESC_CRYPTOGRAPHIC_FAILURE			= 75,
	ESC_CRYPTOGRAPHIC_ALGORITHM_NOT_SUPPORTED	= 76,
	ESC_MESSAGE_INTEGRITY_FAILURE			= 77,
};

enum enhanced_status_class {
	ESC_STATUS_OK		= 2,
	ESC_STATUS_TEMPFAIL	= 4,
	ESC_STATUS_PERMFAIL	= 5,
};

static inline uint32_t
evpid_to_msgid(uint64_t evpid)
{
	return (evpid >> 32);
}

static inline uint64_t
msgid_to_evpid(uint32_t msgid)
{
        return ((uint64_t)msgid << 32);
}

/* dict.c */
#define dict_init(d) do { SPLAY_INIT(&((d)->dict)); (d)->count = 0; } while(0)
#define dict_empty(d) SPLAY_EMPTY(&((d)->dict))
#define dict_count(d) ((d)->count)
int dict_check(struct dict *, const char *);
void *dict_set(struct dict *, const char *, void *);
void dict_xset(struct dict *, const char *, void *);
void *dict_get(struct dict *, const char *);
void *dict_xget(struct dict *, const char *);
void *dict_pop(struct dict *, const char *);
void *dict_xpop(struct dict *, const char *);
int dict_poproot(struct dict *, void **);
int dict_root(struct dict *, const char **, void **);
int dict_iter(struct dict *, void **, const char **, void **);
int dict_iterfrom(struct dict *, void **, const char *, const char **, void **);
void dict_merge(struct dict *, struct dict *);


/* esc.c */
const char *esc_code(enum enhanced_status_class, enum enhanced_status_code);
const char *esc_description(enum enhanced_status_code);


/* filter_api.c */
void  filter_api_session_allocator(void *(*)(uint64_t));
void  filter_api_session_destructor(void (*)(void *));
void *filter_api_session(uint64_t);

void  filter_api_transaction_allocator(void *(*)(uint64_t));
void  filter_api_transaction_destructor(void (*)(void *));
void *filter_api_transaction(uint64_t);

void filter_api_setugid(uid_t, gid_t);
void filter_api_set_chroot(const char *);
void filter_api_no_chroot(void);

void filter_api_data_buffered(void);
void filter_api_data_buffered_stream(uint64_t);

void filter_api_loop(void);
int filter_api_accept(uint64_t);
int filter_api_reject(uint64_t, enum filter_status);
int filter_api_reject_code(uint64_t, enum filter_status, uint32_t,
    const char *);
void filter_api_writeln(uint64_t, const char *);
void filter_api_printf(uint64_t id, const char *, ...);
void filter_api_timer(uint64_t, uint32_t, void (*)(uint64_t, void *), void *);
const char *filter_api_sockaddr_to_text(const struct sockaddr *);
const char *filter_api_mailaddr_to_text(const struct mailaddr *);

FILE *filter_api_datahold_open(uint64_t, void (*callback)(uint64_t, FILE *, void *), void *);
void filter_api_datahold_close(uint64_t);

void filter_api_on_connect(int(*)(uint64_t, struct filter_connect *));
void filter_api_on_helo(int(*)(uint64_t, const char *));
void filter_api_on_mail(int(*)(uint64_t, struct mailaddr *));
void filter_api_on_rcpt(int(*)(uint64_t, struct mailaddr *));
void filter_api_on_data(int(*)(uint64_t));
void filter_api_on_msg_line(void(*)(uint64_t, const char *));
void filter_api_on_msg_start(void(*)(uint64_t));
void filter_api_on_msg_end(int(*)(uint64_t, size_t));
void filter_api_on_reset(void(*)(uint64_t));
void filter_api_on_disconnect(void(*)(uint64_t));
void filter_api_on_tx_begin(void(*)(uint64_t));
void filter_api_on_tx_commit(void(*)(uint64_t));
void filter_api_on_tx_rollback(void(*)(uint64_t));

const char *proc_name(enum smtp_proc_type);
const char *imsg_to_str(int);


/* mproc.c */
int mproc_fork(struct mproc *, const char*, char **);
void mproc_init(struct mproc *, int);
void mproc_clear(struct mproc *);
void mproc_enable(struct mproc *);
void mproc_disable(struct mproc *);
void mproc_event_add(struct mproc *);
void m_compose(struct mproc *, uint32_t, uint32_t, pid_t, int, void *, size_t);
void m_composev(struct mproc *, uint32_t, uint32_t, pid_t, int,
    const struct iovec *, int);
void m_forward(struct mproc *, struct imsg *);
void m_create(struct mproc *, uint32_t, uint32_t, pid_t, int);
void m_add(struct mproc *, const void *, size_t);
void m_add_int(struct mproc *, int);
void m_add_u32(struct mproc *, uint32_t);
void m_add_size(struct mproc *, size_t);
void m_add_time(struct mproc *, time_t);
void m_add_string(struct mproc *, const char *);
void m_add_data(struct mproc *, const void *, size_t);
void m_add_evpid(struct mproc *, uint64_t);
void m_add_msgid(struct mproc *, uint32_t);
void m_add_id(struct mproc *, uint64_t);
void m_add_sockaddr(struct mproc *, const struct sockaddr *);
void m_add_mailaddr(struct mproc *, const struct mailaddr *);
void m_close(struct mproc *);
void m_flush(struct mproc *);

void m_msg(struct msg *, struct imsg *);
int  m_is_eom(struct msg *);
void m_end(struct msg *);
void m_get_int(struct msg *, int *);
void m_get_size(struct msg *, size_t *);
void m_get_u32(struct msg *, uint32_t *);
void m_get_time(struct msg *, time_t *);
void m_get_string(struct msg *, const char **);
void m_get_data(struct msg *, const void **, size_t *);
void m_get_evpid(struct msg *, uint64_t *);
void m_get_msgid(struct msg *, uint32_t *);
void m_get_id(struct msg *, uint64_t *);
void m_get_sockaddr(struct msg *, struct sockaddr *);
void m_get_mailaddr(struct msg *, struct mailaddr *);

#ifdef BUILD_DAEMON
void m_add_envelope(struct mproc *, const struct envelope *);
void m_get_envelope(struct msg *, struct envelope *);
#endif


/* queue */
void queue_api_on_close(int(*)(void));
void queue_api_on_message_create(int(*)(uint32_t *));
void queue_api_on_message_commit(int(*)(uint32_t, const char*));
void queue_api_on_message_delete(int(*)(uint32_t));
void queue_api_on_message_fd_r(int(*)(uint32_t));
void queue_api_on_message_corrupt(int(*)(uint32_t));
void queue_api_on_message_uncorrupt(int(*)(uint32_t));
void queue_api_on_envelope_create(int(*)(uint32_t, const char *, size_t, uint64_t *));
void queue_api_on_envelope_delete(int(*)(uint64_t));
void queue_api_on_envelope_update(int(*)(uint64_t, const char *, size_t));
void queue_api_on_envelope_load(int(*)(uint64_t, char *, size_t));
void queue_api_on_envelope_walk(int(*)(uint64_t *, char *, size_t));
void queue_api_on_message_walk(int(*)(uint64_t *, char *, size_t,
    uint32_t, int *, void **));
void queue_api_no_chroot(void);
void queue_api_set_chroot(const char *);
void queue_api_set_user(const char *);
int queue_api_dispatch(void);

/* queue utils */
uint32_t queue_generate_msgid(void);
uint64_t queue_generate_evpid(uint32_t);
int mktmpfile(void);

/* scheduler */
void scheduler_api_on_init(int(*)(void));
void scheduler_api_on_insert(int(*)(struct scheduler_info *));
void scheduler_api_on_commit(size_t(*)(uint32_t));
void scheduler_api_on_rollback(size_t(*)(uint32_t));
void scheduler_api_on_update(int(*)(struct scheduler_info *));
void scheduler_api_on_delete(int(*)(uint64_t));
void scheduler_api_on_hold(int(*)(uint64_t, uint64_t));
void scheduler_api_on_release(int(*)(int, uint64_t, int));
void scheduler_api_on_batch(int(*)(int, int *, size_t *, uint64_t *, int *));
void scheduler_api_on_messages(size_t(*)(uint32_t, uint32_t *, size_t));
void scheduler_api_on_envelopes(size_t(*)(uint64_t, struct evpstate *, size_t));
void scheduler_api_on_schedule(int(*)(uint64_t));
void scheduler_api_on_remove(int(*)(uint64_t));
void scheduler_api_on_suspend(int(*)(uint64_t));
void scheduler_api_on_resume(int(*)(uint64_t));
void scheduler_api_no_chroot(void);
void scheduler_api_set_chroot(const char *);
void scheduler_api_set_user(const char *);
int scheduler_api_dispatch(void);

/* table */
void table_api_on_update(int(*)(void));
void table_api_on_check(int(*)(int, struct dict *, const char *));
void table_api_on_lookup(int(*)(int, struct dict *, const char *, char *, size_t));
void table_api_on_fetch(int(*)(int, struct dict *, char *, size_t));
int table_api_dispatch(void);
const char *table_api_get_name(void);

/* tree.c */
#define tree_init(t) do { SPLAY_INIT(&((t)->tree)); (t)->count = 0; } while(0)
#define tree_empty(t) SPLAY_EMPTY(&((t)->tree))
#define tree_count(t) ((t)->count)
int tree_check(struct tree *, uint64_t);
void *tree_set(struct tree *, uint64_t, void *);
void tree_xset(struct tree *, uint64_t, void *);
void *tree_get(struct tree *, uint64_t);
void *tree_xget(struct tree *, uint64_t);
void *tree_pop(struct tree *, uint64_t);
void *tree_xpop(struct tree *, uint64_t);
int tree_poproot(struct tree *, uint64_t *, void **);
int tree_root(struct tree *, uint64_t *, void **);
int tree_iter(struct tree *, void **, uint64_t *, void **);
int tree_iterfrom(struct tree *, void **, uint64_t, uint64_t *, void **);
void tree_merge(struct tree *, struct tree *);

/* util.c */
struct iobuf; /* forward declaration */
void	*xmalloc(size_t, const char *);
void	*xcalloc(size_t, size_t, const char *);
char	*xstrdup(const char *, const char *);
void	*xmemdup(const void *, size_t, const char *);
char	*strip(char *);
int	 base64_encode(unsigned char const *, size_t, char *, size_t);
int	 base64_decode(char const *, unsigned char *, size_t);
int	 lowercase(char *, const char *, size_t);

#endif
