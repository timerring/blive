from setuptools import setup


setup(
    name='bilibiliuploader',
    version="0.0.4",
    packages=['bilibiliuploader', 'bilibiliuploader.util'],
    url='https://github.com/FortuneDayssss/BilibiliUploader',
    install_requires=['certifi>=2020.4.5.1',
                      'chardet>=3.0.4',
                      'idna>=2.9',
                      'pyasn1>=0.4.8',
                      'requests>=2.23.0',
                      'rsa>=4.0',
                      'urllib3>=1.25.9',
                      ],
    license='MIT',
    author='FortuneDayssss',
    author_email='717622995@qq.com',
    description='Simulate pc ugc_assistant for bilibili',
    keywords=['bilibili', 'upload'],
    data_files=[('.', ['requirements.txt'])],
    classifiers=[
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python',
        'Topic :: Internet :: WWW/HTTP',
    ],
)
