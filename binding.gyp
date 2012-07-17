{
  'targets': [
    {
      'target_name': 'node-stringprep',
      'sources': [ 'node-stringprep.cc' ],
      'conditions': [
        ['OS=="mac"', {
          'include_dirs': [
              '/opt/local/include'
          ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
     }
  ]
}