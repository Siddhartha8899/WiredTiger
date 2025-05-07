#include <stdio.h>
#include <stdint.h>
#include <errno.h>

/*
 * The WT_SESSION structure is a public-facing structure that provides an
 * abstraction of a thread context for users of WiredTiger.
 */
struct __wt_session {
  char *name;
  int content;
};
typedef struct __wt_session WT_SESSION;

/*
 * The WT_SESSION_IMPL structure contains a public WT_SESSION structure, and
 * adds additional fields that are only used internally by WiredTiger.
 */
struct __wt_session_impl {
  WT_SESSION impl;
  /* Blackbox. */
  WT_TRANSACTION transaction; 
  char *private_content;
};
typedef struct __wt_session_impl WT_SESSION_IMPL;

static int __wt_session_commit_transaction(WT_SESSION *, const char *);
static int txn_commit(WT_SESSION_IMPL *session, const char *config);
static int transaction_rollback(WT_SESSION_IMPL *);

int
main() {
  WT_SESSION_IMPL session;

  // Clears the memory inside session struct.
  memset(&session, 0, sizeof(WT_SESSION_IMPL));

  if (__wt_session_commit_transaction((WT_SESSION*)session, NULL) != 0)
    fprintf(stderr, "Error doing transaction commit\n");

  return 0;
}

/*
 * __wt_session_commit_transaction --
 *      WT_SESSION->commit_transaction method.
 */
static int
__wt_session_commit_transaction(WT_SESSION *wt_session, const char *config)
{
  int ret, t_ret;
  WT_SESSION_IMPL *session;
  WT_TRANSACTION *transaction;

  ret = t_ret = 0;
  session = (WT_SESSION_IMPL *)wt_session;

  transaction = &session->transaction;
  if (transaction->fail == true) {
    fprintf(stderr, "Error doing transaction commit\n");
	return (EINVAL);
  }

  ret = txn_commit(session, config);

  /* Explicitly rollback on error */
  if (ret != 0) {
    if ((t_ret = transaction_rollback(session)) != 0 &&
        ret == 0)
      ret = t_ret;
  }

  return (ret);
}

/* Implementation of commit */
static int
txn_commit(WT_SESSION_IMPL *session, const char *config)
{
  /* Placeholder code. */
  return (EINVAL);
}

/* Implementation of rollback */
static int
transaction_rollback(WT_SESSION_IMPL *session)
{
  /* Placeholder code. */
  return (EINVAL);
}
