#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>

#define ANY  (void*)
int main()
{
	gst_init(NULL, NULL);

	GList* elems = gst_element_factory_list_get_elements(
			GST_ELEMENT_FACTORY_TYPE_ANY, 0);
	{
		void $(GstElementFactory* factory)
		{
			printf("%-20.20s [%-19.19s] %-37.37s\n",
				gst_plugin_feature_get_name(ANY factory),
				gst_element_factory_get_metadata(factory, "klass"),
				gst_element_factory_get_metadata(factory, "long-name"));
		}
		g_list_foreach(elems, ANY $, NULL);
	}
	gst_plugin_feature_list_free(elems);

	return 0;
}

