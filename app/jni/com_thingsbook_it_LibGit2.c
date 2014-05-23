#include <com_thingsbook_it_LibGit2.h>

#include <git2.h>
#include <stdio.h>
#include <assert.h>
#include <pwd.h>

#include <android/log.h>

#define  LOG_TAG    "ItApplication"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// declare check error helping function
static void check_error(int, const char *);

JNIEXPORT jint JNICALL Java_com_thingsbook_it_LibGit2_clone
(JNIEnv * env, jclass cls, jstring url, jstring localPath)
{
	git_threads_init();

	git_repository *cloned_repo = NULL;
	int error;

	LOGD("trying to clone a repository");

	const char *c_url = (*env)->GetStringUTFChars(env, url, NULL);
	const char *c_local_path = (*env)->GetStringUTFChars(env, localPath, NULL);

	LOGD("%s", c_url);
	LOGD("%s", c_local_path);
	
	// print username
	struct passwd *p = getpwuid(getuid());  // Check for NULL!
	LOGD("User name: %s\n", p->pw_name);

	// print libgit2 version
	int major, minor, rev;
	git_libgit2_version(&major, &minor, &rev);
	LOGD("Libgit2 version: %d.%d.%d\n", major, minor, rev);

	// actually call library to clone repository
	error = git_clone(&cloned_repo , c_url, c_local_path, NULL);

	check_error(error, "cloning repository");

	return error;
}

static void check_error(int error_code, const char *action)
{
  const git_error *error = giterr_last();
  if (!error_code)
    return;

  LOGE("Error %d %s - %s\n", error_code, action,
       (error && error->message) ? error->message : "???");

}