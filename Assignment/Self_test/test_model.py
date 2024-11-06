import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
import numpy as np
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix
import os

# Define a simple neural network model
class SimpleMLP(nn.Module):
    def __init__(self, input_size=2, hidden_size1=50, hidden_size2=20, output_size=2):
        super(SimpleMLP, self).__init__()
        self.layers = nn.Sequential(
            nn.Linear(input_size, hidden_size1),
            nn.ReLU(),
            nn.Linear(hidden_size1, hidden_size2),
            nn.ReLU(),
            nn.Linear(hidden_size2, output_size),
            nn.Softmax(dim=1)
        )
    
    def forward(self, x):
        return self.layers(x)
    

# Load data from .npy files
def load_data():
    train_data = np.load('./datasets/2c-classification/2c_train.npy', allow_pickle=True)
    valid_data = np.load('./datasets/2c-classification/2c_valid.npy', allow_pickle=True)
    test_data = np.load('./datasets/2c-classification/2c_test.npy', allow_pickle=True)
    
    X_train, y_train = train_data[:, :-1], train_data[:, -1].astype(np.int64)
    X_valid, y_valid = valid_data[:, :-1], valid_data[:, -1].astype(np.int64)
    X_test, y_test = test_data[:, :-1], test_data[:, -1].astype(np.int64)
    
    return (X_train, y_train), (X_valid, y_valid), (X_test, y_test)

def evaluate_model(model, data_loader):
    model.eval()
    all_preds, all_labels = [], []
    
    # Collect predictions and labels
    with torch.no_grad():
        for inputs, labels in data_loader:
            outputs = model(inputs)
            _, preds = torch.max(outputs, 1)
            all_preds.extend(preds.cpu().numpy())
            all_labels.extend(labels.cpu().numpy())
    
    # Convert to numpy arrays for metric calculations
    y_true = np.array(all_labels)
    y_pred = np.array(all_preds)
    
    # Calculate metrics
    accuracy = accuracy_score(y_true, y_pred)
    precision_macro = precision_score(y_true, y_pred, average='macro')
    recall_macro = recall_score(y_true, y_pred, average='macro')
    f1_macro = f1_score(y_true, y_pred, average='macro')
    
    return accuracy, precision_macro, recall_macro, f1_macro

# Training function
def train_model(model, train_loader, criterion, optimizer, epochs=10, valid_loader=None):
    for epoch in range(epochs):
        model.train()
        for idx, batch in enumerate(train_loader):
            inputs, labels = batch
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            # Print DY after loss.backward()
            optimizer.step()
            with open(os.path.join('./TestLog', 'loss_log.txt'), 'a') as f:
                f.write(f"Epoch [{epoch+1}/{epochs}], Step [{idx+1}/{len(train_loader)}], Loss: {loss.item():.4f}\n")

        accuracy, precision_macro, recall_macro, f1_macro = evaluate_model(model, valid_loader)
        with open(os.path.join('./TestLog', 'loss_log.txt'), 'a') as f:
            f.write(f"Epoch [{epoch+1}/{epochs}], Loss: {loss.item():.4f}\n")
            f.write(f"Validation Accuracy: {accuracy:.4f}, Precision: {precision_macro:.4f}, Recall: {recall_macro:.4f}, F1: {f1_macro:.4f}\n")



# Main function for the classification task
def main():
    (X_train, y_train), (X_valid, y_valid), (X_test, y_test) = load_data()

    # Kiểm tra kích thước dữ liệu
    print("X_train: ", X_train.shape)
    print("y_train: ", y_train.shape)
    print()
    print("X_valid: ", X_valid.shape)
    print("y_valid: ", y_valid.shape)
    print()
    print("X_test: ", X_test.shape)
    print("y_test: ", y_test.shape)

    # Chuẩn hóa dữ liệu dựa trên tập huấn luyện
    mean = X_train.mean(axis=0)
    std = X_train.std()
    # std[std == 0] = 1  # Tránh chia cho 0 nếu có cột hằng số

    X_train = (X_train - mean) / std
    X_valid = (X_valid - mean) / std
    X_test = (X_test - mean) / std

    # Chuyển đổi dữ liệu sang PyTorch tensors
    train_ds = TensorDataset(torch.tensor(X_train, dtype=torch.float32), torch.tensor(y_train))
    valid_ds = TensorDataset(torch.tensor(X_valid, dtype=torch.float32), torch.tensor(y_valid))
    test_ds = TensorDataset(torch.tensor(X_test, dtype=torch.float32), torch.tensor(y_test))

    # Tạo các DataLoader
    train_loader = DataLoader(train_ds, batch_size=50, shuffle=False, drop_last=False)
    valid_loader = DataLoader(valid_ds, batch_size=50, shuffle=False, drop_last=False)
    test_loader = DataLoader(test_ds, batch_size=50, shuffle=False, drop_last=False)
    # Initialize model, criterion, and optimizer
    model = SimpleMLP()
    criterion = nn.CrossEntropyLoss(reduction='mean')
    optimizer = optim.SGD(model.parameters(), lr=0.002)
    
    # Train and evaluate model on validation set
    train_model(model, train_loader, criterion, optimizer, epochs=1000, valid_loader=valid_loader)
    with open(os.path.join('./TestLog', 'loss_log.txt'), 'a') as f:
        f.write("Validation Evaluation:\n")
    evaluate_model(model, valid_loader)
    
    # Evaluate model on test set
    with open(os.path.join('./TestLog', 'loss_log.txt'), 'a') as f:
        f.write("Test Evaluation:\n")
    evaluate_model(model, test_loader)

# Run the classification task
if __name__ == "__main__":
    main()
