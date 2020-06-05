#include <stdio.h>
#include <stdbool.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

static MonoString *sample()
{
	printf("C# invoked a C function\n");
	return mono_string_new(mono_domain_get(), "Hello!");
}

static void *log_call(MonoString *msg)
{
	printf("[log]: %s\n", mono_string_to_utf8(msg));
	return NULL;
}

int main(int argc, char **argv)
{
	// Initialize mono
	MonoDomain *domain = NULL;
	MonoAssembly *assembly = NULL;
	const char *assembly_filename = argv[1];

	printf("Initializing mono\n");

	domain = mono_jit_init("myapp");

	assembly = mono_domain_assembly_open(domain, assembly_filename);
	if (assembly == NULL)
	{
		fprintf(stderr, "Failed to open assembly file %s\n", assembly_filename);
		goto exit;
	}

	mono_add_internal_call("Application::Sample", sample);
	mono_add_internal_call("Application::Log", log_call);

	// Execute Application::Main()
	mono_jit_exec(domain, assembly, argc - 1, argv + 1);
	MonoImage *mono_image = mono_assembly_get_image(assembly);

	/*// Execute Update()
	{
		MonoMethodDesc *mono_method_desc = mono_method_desc_new("Application:Update", false);

		MonoClass *mono_class = mono_class_from_name(mono_image, "", "Application");

		MonoMethod *method = mono_method_desc_search_in_class(mono_method_desc, mono_class);
		void *args[1];
		int frame = 5;
		args[0] = &frame;

		mono_runtime_invoke(method, NULL, args, NULL);
	}*/
	// Create an object
	{
		MonoClass *class = mono_class_from_name(mono_image, "", "Script");
		if (class == NULL)
		{
			printf("Unable to find class\n");
		}

		// Create the instance
		MonoObject *instance = mono_object_new(domain, class);
		mono_runtime_object_init(instance);

		// Invoke string constructor
		MonoMethodDesc *desc = mono_method_desc_new("Script:Update", false);
		MonoMethod *method = mono_method_desc_search_in_class(desc, class);

		void *args[1];
		int frame = 5;
		args[0] = &frame;
		mono_runtime_invoke(method, instance, args, NULL);
	}

exit:
	printf("Exiting mono\n");
	mono_jit_cleanup(domain);
	return EXIT_SUCCESS;
}