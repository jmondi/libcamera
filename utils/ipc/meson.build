# SPDX-License-Identifier: CC0-1.0

subdir('generators')

py_modules += ['jinja2', 'ply']

mojom_parser = find_program('./parser.py')

mojom_generator = find_program('./generate.py')

mojom_templates = custom_target('mojom_templates',
                                input : mojom_template_files,
                                output : 'libcamera_templates.zip',
                                command : [mojom_generator, '-o', '@OUTDIR@', 'precompile'])

mojom_templates_dir = meson.current_build_dir()
