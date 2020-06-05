
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

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

MonoObject *cs_object_create(MonoDomain *domain, MonoImage *assembly_image, const char *name)
{
	MonoClass *class = mono_class_from_name(assembly_image, "", "Script");
	if (class == NULL)
	{
		printf("Unable to find class\n");
	}

	// Create the instance
	MonoObject *object = mono_object_new(domain, class);
	mono_runtime_object_init(object);
	return object;
}

MonoObject *cs_object_invoke(MonoObject *object, const char *name, int param_count, void **params)
{
	MonoClass *class = mono_object_get_class(object);
	MonoMethod *method = mono_class_get_method_from_name(class, name, param_count);
	return mono_runtime_invoke(method, object, params, NULL);
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
	MonoImage *assembly_image = mono_assembly_get_image(assembly);

	MonoObject *script = cs_object_create(domain, assembly_image, "Script");

	int frame = 1;
	while (true)
	{
		void *params[1];
		params[0] = &frame;

		MonoObject* ret = cs_object_invoke(script, "Update", 1, params);
		int a = *(int*)mono_object_unbox(ret);
		printf("Script returned %d\n", a);
		usleep(100000);
		frame++;
	}
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
	}

exit:
	printf("Exiting mono\n");
	mono_jit_cleanup(domain);
	return EXIT_SUCCESS;
}