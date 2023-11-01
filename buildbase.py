#!/usr/bin/env python3
from __future__ import annotations
import sys
import argparse
import subprocess
import datetime
import os

from typing import Optional

def clone_git_repos(build_dir : str, force_clone : bool) -> bool:
    # Make the build directory
    os.makedirs(build_dir,exist_ok=True)
    if not os.path.isdir(f'{build_dir}/riscv-gnu-toolchain/.git') or force_clone:
        print(f'Cloning into {build_dir}')
        current_dir = os.getcwd()
        os.chdir(build_dir)
        with subprocess.Popen(['git','clone','--recursive','https://github.com/riscv/riscv-gnu-toolchain'], stdout=subprocess.PIPE) as git_clone_proc:
            for line in git_clone_proc.stdout.readlines():
                line_str = line.decode('utf-8')
                print(f'  {line_str}')
        os.chdir(current_dir)
        git_clone_proc.wait()
        if git_clone_proc.returncode:
            return False
    else:
        print(f'Repository appears to exist in {build_dir}')
    os.makedirs(f'{build_dir}/riscv-gnu-toolchain/build',exist_ok=True)
    return True
    

def builderx_builder_exists(builder_name : str) -> bool:
    command = ['docker','buildx','ls']
    found_container = False
    with subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL) as docker_ls_proc:
        for line in docker_ls_proc.stdout.readlines():
            split_line = line.decode('utf-8').split()
            if len(split_line) and str(split_line[0]) == builder_name:
                found_container = True
    return found_container

def create_use_builderx_builder(builder_name : str) -> bool:
    if not builderx_builder_exists(builder_name=builder_name):
        print(f'Creating buildx builder {builder_name}')
        command = ['docker','buildx','create','--name',builder_name,'--use']
    else:
        print(f'Selecting to use buildx builder {builder_name}')
        command = ['docker','buildx','use',builder_name]
    with subprocess.Popen(command, stdout=subprocess.PIPE) as docker_buildx_creater:
        for line in docker_buildx_creater.stdout.readlines():
            line_str = line.decode('utf-8')
            print(f'  {line_str}')
        docker_buildx_creater.wait()
        return not docker_buildx_creater.returncode

def build_docker_images(repo : str, tag : str) -> bool:
    command = ['docker','buildx','build','--platform','linux/amd64,linux/arm64','--push','-t',f'{repo}:latest','-t',f'{repo}:{tag}','-f','Dockerfile.base','.']
    print(f'Building images {repo}:{tag}')
    with subprocess.Popen(command, stdout=subprocess.PIPE) as docker_buildx_builder:
        for line in docker_buildx_builder.stdout.readlines():
            line_str = line.decode('utf-8')
            print(f'  {line_str}')
        docker_buildx_builder.wait()
        return not docker_buildx_builder.returncode

def remove_builderx_builder(builder_name : str) -> bool:
    print(f'Stopping builder {builder_name}')
    command = ['docker','buildx','stop',builder_name]
    with subprocess.Popen(command, stdout=subprocess.PIPE) as docker_buildx_stopper:
        for line in docker_buildx_stopper.stdout.readlines():
            line_str = line.decode('utf-8')
            print(f'  {line_str}')
        docker_buildx_stopper.wait()
        if docker_buildx_stopper.returncode:
            return False
    print(f'Removing builder {builder_name}')
    command = ['docker','buildx','rm',builder_name]
    with subprocess.Popen(command, stdout=subprocess.PIPE) as docker_buildx_remover:
        for line in docker_buildx_remover.stdout.readlines():
            line_str = line.decode('utf-8')
            print(f'  {line_str}')
        docker_buildx_remover.wait()
        if docker_buildx_remover.returncode:
            return False
    return True

def main(*argv):
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('-b','--builder',default='riscv_base_builder')
    arg_parser.add_argument('-d','--dir',default='build')
    arg_parser.add_argument('-r','--repo',default='cjnitta/riscv_base')
    arg_parser.add_argument('-f','--force',action='store_true')
    arg_parser.add_argument('-t','--tag',default=None)
    arguments = arg_parser.parse_args(list(argv))
    tag : Optional[str] = arguments.tag
    if tag is None:
        now = datetime.datetime.now()
        tag = datetime.datetime.strftime(now,'%Y-%m-%d')
    #build_dir = arguments.dir if arguments.dir[-1] !=  '/' else arguments.dir[:-1]
    #if not clone_git_repos(build_dir=build_dir,force_clone=arguments.force):
    #    return -1
    if not create_use_builderx_builder(builder_name=arguments.builder):
        return -1
    if not build_docker_images(repo=arguments.repo,tag=tag):
        return -1
    if not remove_builderx_builder(builder_name=arguments.builder):
        return -1
    
    return 0

if __name__ == '__main__':
    sys.exit(main(*sys.argv[1:]))