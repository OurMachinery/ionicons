// Generates the ionicons.h header file. Run for example as:
//
// zig cc generate_header.c -I%TM_SDK_DIR%/headers -DTM_LINKS_FOUNDATION  %TM_SDK_DIR%/lib/vs2019/Debug/foundation.lib && a.out

#include <foundation/allocator.h>
#include <foundation/carray_print.inl>
#include <foundation/config.h>
#include <foundation/json.h>
#include <foundation/os.h>
#include <foundation/temp_allocator.h>
#include <foundation/unicode.h>

#include <stdio.h>

static void generate_from_json(const char *path, const char *prefix, char **out, tm_allocator_i *a)
{
    TM_INIT_TEMP_ALLOCATOR(ta);

    tm_carray_print(out, a, "\nenum {\n");

    const uint64_t size = tm_os_api->file_system->stat(path).size;
    char *json = tm_temp_alloc(ta, size + 1);
    json[size] = 0;

    tm_file_o f = tm_os_api->file_io->open_input(path);
    tm_os_api->file_io->read(f, json, size);
    tm_os_api->file_io->close(f);

    tm_config_i *cd = tm_config_api->create(a);
    tm_json_api->parse(json, cd, 0, 0);
    tm_config_item_t root = cd->root(cd->inst);
    tm_config_item_t *keys, *values;
    const uint32_t n = cd->to_object(cd->inst, root, &keys, &values);
    uint32_t next = 0;
    for (uint32_t i = 0; i < n; ++i) {
        const char *name = cd->to_string(cd->inst, keys[i]);
        const uint32_t glyph = cd->to_number(cd->inst, values[i]);

        // Ignore the iOS icons.
        if (strlen(name) > 4 && memcmp(name, "ios-", 4) == 0)
            break;

        char *name_caps = tm_temp_alloc(ta, strlen(name) + 1);
        memcpy(name_caps, name, strlen(name) + 1);
        for (char *s = name_caps; *s; ++s) {
            if (*s >= 'a' && *s <= 'z')
                *s = *s - 'a' + 'A';
            else if (*s == '-')
                *s = '_';
        }

        if (glyph == next)
            tm_carray_printf(out, a, "    %s__%s,\n", prefix, name_caps);
        else
            tm_carray_printf(out, a, "    %s__%s = %d,\n", prefix, name_caps, glyph);
        next = glyph + 1;
    }

    tm_carray_print(out, a, "};\n");

    TM_SHUTDOWN_TEMP_ALLOCATOR(ta);
}

int main(int argc, const char **argv)
{
    TM_INIT_TEMP_ALLOCATOR_WITH_ADAPTER(ta, a);

    char *out = 0;

    tm_carray_print(&out, a,
        "// This header file lists all the glyphs in the ionicons.ttf file included in this\n"
        "// library package.\n");

    generate_from_json("ionicons.json", "IONICON", &out, a);
    generate_from_json("the-machinery-add-ons.json", "IONICON_ADD_ON", &out, a);

    tm_file_o of = tm_os_api->file_io->open_output("ionicons.h", false);
    tm_os_api->file_io->write(of, out, tm_carray_size(out));
    tm_os_api->file_io->close(of);

    TM_SHUTDOWN_TEMP_ALLOCATOR(ta);
}