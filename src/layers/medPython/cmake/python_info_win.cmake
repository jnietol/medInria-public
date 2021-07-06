set(PYTHON_VERSION_MINOR 9)
set(PYTHON_MAIN_LIBRARY python39.dll)
set(PYTHON_HEADERS_DIR headers/python3.9)
set(PYTHON_LIBRARIES_DIR libraries/python3.9)
set(PYTHON_MODULES_DIR modules/python3.9)
set(PYTHON_LICENSE_FILE LICENSE.txt)

set(PYTHON_HEADERS
    Python-ast.h
    Python.h
    abstract.h
    asdl.h
    ast.h
    bitset.h
    bltinmodule.h
    boolobject.h
    bytearrayobject.h
    bytesobject.h
    cellobject.h
    ceval.h
    classobject.h
    code.h
    codecs.h
    compile.h
    complexobject.h
    context.h
    cpython
    cpython/abstract.h
    cpython/bytearrayobject.h
    cpython/bytesobject.h
    cpython/ceval.h
    cpython/code.h
    cpython/dictobject.h
    cpython/fileobject.h
    cpython/fileutils.h
    cpython/frameobject.h
    cpython/import.h
    cpython/initconfig.h
    cpython/interpreteridobject.h
    cpython/listobject.h
    cpython/methodobject.h
    cpython/object.h
    cpython/objimpl.h
    cpython/pyerrors.h
    cpython/pylifecycle.h
    cpython/pymem.h
    cpython/pystate.h
    cpython/sysmodule.h
    cpython/traceback.h
    cpython/tupleobject.h
    cpython/unicodeobject.h
    datetime.h
    descrobject.h
    dictobject.h
    dynamic_annotations.h
    enumobject.h
    errcode.h
    eval.h
    exports.h
    fileobject.h
    fileutils.h
    floatobject.h
    frameobject.h
    funcobject.h
    genericaliasobject.h
    genobject.h
    graminit.h
    grammar.h
    import.h
    internal
    internal/pegen_interface.h
    internal/pycore_abstract.h
    internal/pycore_accu.h
    internal/pycore_atomic.h
    internal/pycore_bytes_methods.h
    internal/pycore_byteswap.h
    internal/pycore_call.h
    internal/pycore_ceval.h
    internal/pycore_code.h
    internal/pycore_condvar.h
    internal/pycore_context.h
    internal/pycore_dtoa.h
    internal/pycore_fileutils.h
    internal/pycore_gc.h
    internal/pycore_getopt.h
    internal/pycore_gil.h
    internal/pycore_hamt.h
    internal/pycore_hashtable.h
    internal/pycore_import.h
    internal/pycore_initconfig.h
    internal/pycore_interp.h
    internal/pycore_object.h
    internal/pycore_pathconfig.h
    internal/pycore_pyerrors.h
    internal/pycore_pyhash.h
    internal/pycore_pylifecycle.h
    internal/pycore_pymem.h
    internal/pycore_pystate.h
    internal/pycore_runtime.h
    internal/pycore_sysmodule.h
    internal/pycore_traceback.h
    internal/pycore_tupleobject.h
    internal/pycore_warnings.h
    interpreteridobject.h
    intrcheck.h
    iterobject.h
    listobject.h
    longintrepr.h
    longobject.h
    marshal.h
    memoryobject.h
    methodobject.h
    modsupport.h
    moduleobject.h
    namespaceobject.h
    node.h
    object.h
    objimpl.h
    odictobject.h
    opcode.h
    osdefs.h
    osmodule.h
    parsetok.h
    patchlevel.h
    picklebufobject.h
    py_curses.h
    pyarena.h
    pycapsule.h
    pyconfig.h
    pyctype.h
    pydebug.h
    pydtrace.h
    pyerrors.h
    pyexpat.h
    pyfpe.h
    pyframe.h
    pyhash.h
    pylifecycle.h
    pymacconfig.h
    pymacro.h
    pymath.h
    pymem.h
    pyport.h
    pystate.h
    pystrcmp.h
    pystrhex.h
    pystrtod.h
    pythonrun.h
    pythread.h
    pytime.h
    rangeobject.h
    setobject.h
    sliceobject.h
    structmember.h
    structseq.h
    symtable.h
    sysmodule.h
    token.h
    traceback.h
    tracemalloc.h
    tupleobject.h
    typeslots.h
    ucnhash.h
    unicodeobject.h
    warnings.h
    weakrefobject.h
    )

set(PYTHON_LIBRARIES
    _asyncio.pyd
    _bz2.pyd
    _ctypes.pyd
    _decimal.pyd
    _elementtree.pyd
    _hashlib.pyd
    _lzma.pyd
    _msi.pyd
    _multiprocessing.pyd
    _overlapped.pyd
    _queue.pyd
    _socket.pyd
    _sqlite3.pyd
    _ssl.pyd
    _uuid.pyd
    _zoneinfo.pyd
    libcrypto-1_1.dll
    libffi-7.dll
    libssl-1_1.dll
    pyexpat.pyd
    python3.dll
    python39.dll
    select.pyd
    sqlite3.dll
    unicodedata.pyd
    vcruntime140.dll
    vcruntime140_1.dll
    winsound.pyd
    )

set(PYTHON_MODULES
    __future__.pyc
    __phello__.foo.pyc
    _aix_support.pyc
    _bootlocale.pyc
    _bootsubprocess.pyc
    _collections_abc.pyc
    _compat_pickle.pyc
    _compression.pyc
    _markupbase.pyc
    _osx_support.pyc
    _py_abc.pyc
    _pydecimal.pyc
    _pyio.pyc
    _sitebuiltins.pyc
    _strptime.pyc
    _threading_local.pyc
    _weakrefset.pyc
    abc.pyc
    aifc.pyc
    antigravity.pyc
    argparse.pyc
    ast.pyc
    asynchat.pyc
    asyncore.pyc
    base64.pyc
    bdb.pyc
    binhex.pyc
    bisect.pyc
    bz2.pyc
    cProfile.pyc
    calendar.pyc
    cgi.pyc
    cgitb.pyc
    chunk.pyc
    cmd.pyc
    code.pyc
    codecs.pyc
    codeop.pyc
    colorsys.pyc
    compileall.pyc
    configparser.pyc
    contextlib.pyc
    contextvars.pyc
    copy.pyc
    copyreg.pyc
    crypt.pyc
    csv.pyc
    dataclasses.pyc
    datetime.pyc
    decimal.pyc
    difflib.pyc
    dis.pyc
    doctest.pyc
    encodings___init__.pyc
    encodings_aliases.pyc
    encodings_ascii.pyc
    encodings_base64_codec.pyc
    encodings_big5.pyc
    encodings_big5hkscs.pyc
    encodings_bz2_codec.pyc
    encodings_charmap.pyc
    encodings_cp037.pyc
    encodings_cp1006.pyc
    encodings_cp1026.pyc
    encodings_cp1125.pyc
    encodings_cp1140.pyc
    encodings_cp1250.pyc
    encodings_cp1251.pyc
    encodings_cp1252.pyc
    encodings_cp1253.pyc
    encodings_cp1254.pyc
    encodings_cp1255.pyc
    encodings_cp1256.pyc
    encodings_cp1257.pyc
    encodings_cp1258.pyc
    encodings_cp273.pyc
    encodings_cp424.pyc
    encodings_cp437.pyc
    encodings_cp500.pyc
    encodings_cp720.pyc
    encodings_cp737.pyc
    encodings_cp775.pyc
    encodings_cp850.pyc
    encodings_cp852.pyc
    encodings_cp855.pyc
    encodings_cp856.pyc
    encodings_cp857.pyc
    encodings_cp858.pyc
    encodings_cp860.pyc
    encodings_cp861.pyc
    encodings_cp862.pyc
    encodings_cp863.pyc
    encodings_cp864.pyc
    encodings_cp865.pyc
    encodings_cp866.pyc
    encodings_cp869.pyc
    encodings_cp874.pyc
    encodings_cp875.pyc
    encodings_cp932.pyc
    encodings_cp949.pyc
    encodings_cp950.pyc
    encodings_euc_jis_2004.pyc
    encodings_euc_jisx0213.pyc
    encodings_euc_jp.pyc
    encodings_euc_kr.pyc
    encodings_gb18030.pyc
    encodings_gb2312.pyc
    encodings_gbk.pyc
    encodings_hex_codec.pyc
    encodings_hp_roman8.pyc
    encodings_hz.pyc
    encodings_idna.pyc
    encodings_iso2022_jp.pyc
    encodings_iso2022_jp_1.pyc
    encodings_iso2022_jp_2.pyc
    encodings_iso2022_jp_2004.pyc
    encodings_iso2022_jp_3.pyc
    encodings_iso2022_jp_ext.pyc
    encodings_iso2022_kr.pyc
    encodings_iso8859_1.pyc
    encodings_iso8859_10.pyc
    encodings_iso8859_11.pyc
    encodings_iso8859_13.pyc
    encodings_iso8859_14.pyc
    encodings_iso8859_15.pyc
    encodings_iso8859_16.pyc
    encodings_iso8859_2.pyc
    encodings_iso8859_3.pyc
    encodings_iso8859_4.pyc
    encodings_iso8859_5.pyc
    encodings_iso8859_6.pyc
    encodings_iso8859_7.pyc
    encodings_iso8859_8.pyc
    encodings_iso8859_9.pyc
    encodings_johab.pyc
    encodings_koi8_r.pyc
    encodings_koi8_t.pyc
    encodings_koi8_u.pyc
    encodings_kz1048.pyc
    encodings_latin_1.pyc
    encodings_mac_arabic.pyc
    encodings_mac_croatian.pyc
    encodings_mac_cyrillic.pyc
    encodings_mac_farsi.pyc
    encodings_mac_greek.pyc
    encodings_mac_iceland.pyc
    encodings_mac_latin2.pyc
    encodings_mac_roman.pyc
    encodings_mac_romanian.pyc
    encodings_mac_turkish.pyc
    encodings_mbcs.pyc
    encodings_oem.pyc
    encodings_palmos.pyc
    encodings_ptcp154.pyc
    encodings_punycode.pyc
    encodings_quopri_codec.pyc
    encodings_raw_unicode_escape.pyc
    encodings_rot_13.pyc
    encodings_shift_jis.pyc
    encodings_shift_jis_2004.pyc
    encodings_shift_jisx0213.pyc
    encodings_tis_620.pyc
    encodings_undefined.pyc
    encodings_unicode_escape.pyc
    encodings_utf_16.pyc
    encodings_utf_16_be.pyc
    encodings_utf_16_le.pyc
    encodings_utf_32.pyc
    encodings_utf_32_be.pyc
    encodings_utf_32_le.pyc
    encodings_utf_7.pyc
    encodings_utf_8.pyc
    encodings_utf_8_sig.pyc
    encodings_uu_codec.pyc
    encodings_zlib_codec.pyc
    enum.pyc
    filecmp.pyc
    fileinput.pyc
    fnmatch.pyc
    formatter.pyc
    fractions.pyc
    ftplib.pyc
    functools.pyc
    genericpath.pyc
    getopt.pyc
    getpass.pyc
    gettext.pyc
    glob.pyc
    graphlib.pyc
    gzip.pyc
    hashlib.pyc
    heapq.pyc
    hmac.pyc
    imaplib.pyc
    imghdr.pyc
    imp.pyc
    inspect.pyc
    io.pyc
    ipaddress.pyc
    keyword.pyc
    linecache.pyc
    locale.pyc
    lzma.pyc
    mailbox.pyc
    mailcap.pyc
    mimetypes.pyc
    modulefinder.pyc
    netrc.pyc
    nntplib.pyc
    ntpath.pyc
    nturl2path.pyc
    numbers.pyc
    opcode.pyc
    operator.pyc
    optparse.pyc
    os.pyc
    packages.zip
    pathlib.pyc
    pdb.pyc
    pickle.pyc
    pickletools.pyc
    pipes.pyc
    pkgutil.pyc
    platform.pyc
    plistlib.pyc
    poplib.pyc
    posixpath.pyc
    pprint.pyc
    profile.pyc
    pstats.pyc
    pty.pyc
    py_compile.pyc
    pyclbr.pyc
    pydoc.pyc
    queue.pyc
    quopri.pyc
    random.pyc
    re.pyc
    reprlib.pyc
    rlcompleter.pyc
    runpy.pyc
    sched.pyc
    secrets.pyc
    selectors.pyc
    shelve.pyc
    shlex.pyc
    shutil.pyc
    signal.pyc
    site.pyc
    smtpd.pyc
    smtplib.pyc
    sndhdr.pyc
    socket.pyc
    socketserver.pyc
    sre_compile.pyc
    sre_constants.pyc
    sre_parse.pyc
    ssl.pyc
    stat.pyc
    statistics.pyc
    string.pyc
    stringprep.pyc
    struct.pyc
    subprocess.pyc
    sunau.pyc
    symbol.pyc
    symtable.pyc
    sysconfig.pyc
    tabnanny.pyc
    tarfile.pyc
    telnetlib.pyc
    tempfile.pyc
    textwrap.pyc
    this.pyc
    threading.pyc
    timeit.pyc
    token.pyc
    tokenize.pyc
    trace.pyc
    traceback.pyc
    tracemalloc.pyc
    tty.pyc
    types.pyc
    typing.pyc
    uu.pyc
    uuid.pyc
    warnings.pyc
    wave.pyc
    weakref.pyc
    webbrowser.pyc
    xdrlib.pyc
    zipapp.pyc
    zipfile.pyc
    zipimport.pyc
    )
