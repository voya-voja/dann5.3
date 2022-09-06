# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
# This allows you to connect to the Workspace you've previously deployed in Azure.
# Be sure to fill in the settings below which can be retrieved by running 'az quantum workspace show' in the terminal.
from azure.quantum import Workspace

# Copy the settings for nebojsa@nand5.onmicrosoft.com Azure workspace below
workspace = Workspace (
  subscription_id = "68723a8d-6a87-48ae-9d27-a175239ebd1b",
  resource_group = "d5o",
  name = "d5oWS",
  location = "eastus"
)
