#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>

#define ANY  (void*)
int main()
{
	gst_init(NULL, NULL);

	//GstElement* pipeline = gst_pipeline_new("test");
	GstElement* pipeline = gst_element_factory_make("pipeline", NULL);

	GstElement* video = gst_element_factory_make("videotestsrc", NULL);
	g_object_set(video, "pattern", 0, NULL);
	gst_bin_add(ANY pipeline, video);

	GstElement* display = gst_element_factory_make("xvimagesink", NULL);
	gst_bin_add(ANY pipeline, display);

	gst_element_link(video, display);
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	sleep(2);
	return 0;
}

